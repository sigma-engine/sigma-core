from compile_database import compile_database
from glob import glob
import argparse
import clang_patch
import clang.cindex
import jinja2
import os
import re
import sys
import time
import json


IGNORE_NAMESPACES = ["::std", "std", "boost", "mpl_", "literals",
                     "chrono_literals", "__gnu_cxx", "__cxxabiv1", "Json", "glm"]


def roundUp(number, multiple):
    number = int(number)
    multiple = int(multiple)
    return ((number + multiple - 1) / multiple) * multiple


def make_vaild_identifier(string):
    return re.sub('\W|^(?=\d)', '_', string)


def replace_all(strings, with_string, in_string):
    for s in strings:
        in_string = in_string.replace(s, with_string)
    return in_string


def full_name(cursor):
    name = cursor.spelling
    while cursor.semantic_parent and cursor.semantic_parent.kind != clang.cindex.CursorKind.TRANSLATION_UNIT:
        cursor = cursor.semantic_parent
        name = cursor.spelling + "::" + name
    return name


class reflection_type(object):
    def __init__(self, typet, element_count=0):
        if not isinstance(typet, clang.cindex.Type):
            self.is_array = False
            self.name = typet
            if element_count > 0:
                self.is_array = True
                self.element_count = element_count
        else:
            self.is_array = False
            self.name = typet.spelling
            if typet.kind == clang.cindex.TypeKind.CONSTANTARRAY:
                self.is_array = True
                self.name = typet.element_type.spelling
                self.element_count = typet.element_count


class std140_type(object):
    def __init__(self, type):
        self.name = type.name
        self.name = type.name.replace("glm::", "")
        self.is_array = type.is_array

        count = 1
        if type.is_array:
            self.element_count = type.element_count
            self.element_type = std140_type(reflection_type(self.name, 0))

    def alignment(self):
        N = 4
        if not self.is_array:
            for typename, basic_unit_count in [("float", 1),
                                               ("vec2", 2), ("vec3", 4), ("vec4", 4),
                                               ("mat2", 4), ("mat3", 4), ("mat4", 4)]:
                if self.name == typename:
                    return basic_unit_count * N

            raise NotImplementedError(
                "alignment not implemented for type '{}'".format(self.name))
        else:
            return roundUp(self.element_type.alignment(), 4 * N)

    def size(self):
        N = 4
        for typename, basic_unit_count in [("float", 1),
                                           ("vec2", 2), ("vec3", 3), ("vec4", 4),
                                           ("mat2", 2 * 4), ("mat3", 3 * 4), ("mat4", 4 * 4)]:
            if self.name == typename:
                size = basic_unit_count * N

        if not self.is_array:
            return size
        else:
            return self.element_count * roundUp(size, 4 * N)

    def align_to_start(self, offset):
        return roundUp(offset, self.alignment())

    def align_to_end(self, start_offset):
        if self.is_array:
            return roundUp(start_offset + self.size(), self.alignment())
        else:
            return start_offset + self.size()


class reflection_decleration(object):
    def __init__(self, source_directory, source_file, annotations=[]):
        self.source_directory = source_directory
        self.file = str(source_file)
        for a in annotations:
            value = True
            if '=' in a:
                a = a.split('=')
                value = a[1]
                a = a[0]
            setattr(self, a.strip(), value)


class reflection_field(object):
    def __init__(self, cursor):
        self.name = cursor.spelling
        self.type = reflection_type(cursor.type)
        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.ANNOTATE_ATTR:
                for a in child.spelling.split(","):
                    setattr(self, a.strip(), True)


class reflection_enum(reflection_decleration):
    def __init__(self, source_directory, cursor, annotations):
        super(reflection_enum, self).__init__(source_directory, cursor.location.file, annotations)
        self.name = full_name(cursor)
        self.constants = []
        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.ENUM_CONSTANT_DECL:
                self.constants.append(child.spelling)

    def __str__(self):
        return self.name


class reflection_class(reflection_decleration):
    def __init__(self, source_directory, cursor, annotations):
        super(reflection_class, self).__init__(source_directory, cursor.location.file, annotations)
        self.name = full_name(cursor)
        self.fields = []
        for child in cursor.get_children():
            if child.access_specifier != clang.cindex.AccessSpecifier.PUBLIC:
                continue
            if child.kind == clang.cindex.CursorKind.FIELD_DECL:
                self.fields.append(reflection_field(child))

        if hasattr(self, 'std140') and self.std140:
            self.glsl_name = self.name
            if '::' in self.glsl_name:
                self.glsl_name = self.glsl_name.split("::")[-1]

            offset = 0
            for field in self.fields:
                field.std140_type = std140_type(field.type)
                field.std140_offset = field.std140_type.align_to_start(offset)
                offset = field.std140_type.align_to_end(field.std140_offset)
            self.std140_size = offset

    def __str__(self):
        return self.name


class reflection_world(reflection_decleration):
    def __init__(self, source_directory, cursor, canonical):
        super(reflection_world, self).__init__(source_directory, cursor.location.file)
        self.name = full_name(cursor)
        self.components = [canonical.get_template_argument_type(
            i).spelling for i in range(canonical.get_num_template_arguments())]

    def __str__(self):
        return self.name


class reflection_database:
    def __init__(self, source_directory):
        self.source_directory = source_directory

    def extract(self, cursor):
        self.enums = []
        self.classes = []
        self.worlds = []
        self.__extract_exports(cursor, None)
        return len(self.enums) > 0 or len(self.classes) > 0 or len(self.worlds) > 0

    def __extract_exports(self, cursor, parent):
        if "__" in cursor.spelling:  # 2.10.3.1
            return
        elif cursor.kind in [clang.cindex.CursorKind.NAMESPACE, clang.cindex.CursorKind.NAMESPACE_REF]:
            fname = full_name(cursor)
            for ign in IGNORE_NAMESPACES:
                if fname.startswith(ign):
                    return
        elif cursor.kind == clang.cindex.CursorKind.ANNOTATE_ATTR:
            annotations = {a for a in cursor.spelling.split(',') if a != ''}
            if 'R_EXPORT' in annotations:
                annotations.remove('R_EXPORT')
                if parent.location.is_from_main_file():
                    if parent.kind in [clang.cindex.CursorKind.CLASS_DECL, clang.cindex.CursorKind.STRUCT_DECL]:
                        self.classes.append(reflection_class(
                            self.source_directory, parent, annotations))
                    elif parent.kind == clang.cindex.CursorKind.ENUM_DECL:
                        self.enums.append(reflection_enum(
                            self.source_directory, parent, annotations))
                return
        elif cursor.kind in [clang.cindex.CursorKind.DECL_REF_EXPR,
                             clang.cindex.CursorKind.FUNCTION_DECL,
                             clang.cindex.CursorKind.CXX_METHOD,
                             clang.cindex.CursorKind.CONSTRUCTOR,
                             clang.cindex.CursorKind.DESTRUCTOR,
                             clang.cindex.CursorKind.OVERLOADED_DECL_REF,
                             clang.cindex.CursorKind.CLASS_TEMPLATE,
                             clang.cindex.CursorKind.FUNCTION_TEMPLATE,
                             clang.cindex.CursorKind.CONVERSION_FUNCTION,
                             clang.cindex.CursorKind.CLASS_TEMPLATE_PARTIAL_SPECIALIZATION,
                             clang.cindex.CursorKind.PAREN_EXPR]:
            return
        elif cursor.kind == clang.cindex.CursorKind.TYPE_ALIAS_DECL:
            canonical_type = cursor.type.get_canonical()
            type_decl = canonical_type.get_declaration()
            if type_decl.kind != clang.cindex.CursorKind.NO_DECL_FOUND:
                if full_name(type_decl) == "sigma::world":
                    if cursor.location.is_from_main_file():
                        self.worlds.append(reflection_world(
                            self.source_directory, cursor, canonical_type))
            return

        for child in cursor.get_children():
            self.__extract_exports(child, cursor)


def generated_metadata(database, header_file):
    metadata_file = header_file.replace(
        database.source_directory, database.build_directory) + ".meta"

    output_directory = os.path.dirname(metadata_file)
    if not os.path.exists(output_directory):
        try:
            os.makedirs(output_directory)
        except:  # keep from failing when this script is run in parallel
            if not os.path.exists(output_directory):
                raise

    reflection_db = reflection_database(database.source_directory)
    index = clang.cindex.Index.create(True)
    tu = database.parse(index, header_file)
    if reflection_db.extract(tu.cursor):
        with open(metadata_file, 'w') as output_file:
            output_file.write(json.dumps(reflection_db, indent=4,
                                         sort_keys=True, default=lambda o: o.__dict__))
        return reflection_db


def render_metadata_template(reflection_db, template_file_path, header_file):
    template_name = os.path.splitext(os.path.basename(template_file_path))
    template_name = template_name[0].replace(".j2", "")

    generated_file = header_file.replace(database.source_directory, database.build_directory)
    generated_file = os.path.splitext(generated_file)[0] + "." + template_name

    generated_gaurd = replace_all(
        [database.build_directory.lower() + "/", "include/", "inc/", "src/"], "", generated_file.lower())
    generated_gaurd = make_vaild_identifier(generated_gaurd.upper())

    if reflection_db:
        with open(template_file_path, "r") as template_file:
            template = jinja2.Template(template_file.read())

            with open(generated_file, 'w') as output:
                output.write(template.render(
                    generated_gaurd=generated_gaurd, database=reflection_db) + "\n")
    else:
        sys.exit(-1)


parser = argparse.ArgumentParser(description='Generate C/C++ reflection metadata.')
parser.add_argument('-s', '--source-directory', type=str,
                    help='The root source folder of the project.', required=True)
parser.add_argument('-b', '--build-directory', type=str,
                    help='The root build folder of the project.', required=True)
parser.add_argument('-f', '--file', type=str,
                    help='The header to generate the metadata for.', required=True)

sub_parsers = parser.add_subparsers(dest="sub_parser")

generate_parser = sub_parsers.add_parser("generate")

render_template_parser = sub_parsers.add_parser("template")
render_template_parser.add_argument('-t', '--template-file', type=str,
                                    help='The template file used to generate the metadata.', required=True)

args = parser.parse_args()

header_file = os.path.abspath(args.file)
database = compile_database(os.path.abspath(args.source_directory),
                            os.path.abspath(args.build_directory))


if args.sub_parser == "generate":
    generated_metadata(database, header_file)
elif args.sub_parser == "template":
    metadata_file = header_file.replace(
        database.source_directory, database.build_directory) + ".meta"
    with open(metadata_file, 'r') as input_meta:
        reflection_db = json.load(input_meta)
        render_metadata_template(reflection_db, os.path.abspath(args.template_file), header_file)
