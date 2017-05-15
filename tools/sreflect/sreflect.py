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


class reflection_field:
    def __init__(self, cursor):
        self.name = cursor.spelling
        self.type = cursor.type.spelling
        self.attributes = []
        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.ANNOTATE_ATTR:
                self.attributes += [a.strip() for a in child.spelling.split(",")]

    def is_derived(self):
        return "derived" in self.attributes


class reflection_decleration(object):
    def __init__(self, source_directory, source_file):
        self.source_directory = source_directory
        self.file = str(source_file)


class reflection_enum(reflection_decleration):
    def __init__(self, source_directory, cursor):
        super(reflection_enum, self).__init__(source_directory, cursor.location.file)
        self.name = full_name(cursor)
        self.constants = []
        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.ENUM_CONSTANT_DECL:
                self.constants.append(child.spelling)

    def __str__(self):
        return self.name


class reflection_class(reflection_decleration):
    def __init__(self, source_directory, cursor):
        super(reflection_class, self).__init__(source_directory, cursor.location.file)
        self.name = full_name(cursor)
        self.fields = []
        for child in cursor.get_children():
            if child.access_specifier != clang.cindex.AccessSpecifier.PUBLIC:
                continue
            if child.kind == clang.cindex.CursorKind.FIELD_DECL:
                self.fields.append(reflection_field(child))

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
        self.ignore_namespaces = ["::std", "std", "boost", "mpl_", "literals",
                                  "chrono_literals", "__gnu_cxx", "__cxxabiv1", "Json", "glm"]
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
            for ign in self.ignore_namespaces:
                if fname.startswith(ign):
                    return

        elif cursor.kind == clang.cindex.CursorKind.ANNOTATE_ATTR and cursor.spelling in ["R_CLASS", "R_ENUM"]:
            if parent.location.is_from_main_file():
                if parent.kind in [clang.cindex.CursorKind.CLASS_DECL, clang.cindex.CursorKind.STRUCT_DECL]:
                    self.classes.append(reflection_class(self.source_directory, parent))
                elif parent.kind == clang.cindex.CursorKind.ENUM_DECL:
                    self.enums.append(reflection_enum(self.source_directory, parent))
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

        # print cursor.kind, cursor.spelling
        for child in cursor.get_children():
            self.__extract_exports(child, cursor)


def render_metadata_template(database, template_file_path, header_file):
    template_name = os.path.splitext(os.path.basename(template_file_path))[0]

    generated_header_file = os.path.splitext(
        header_file.replace(database.source_directory, database.build_directory))
    generated_header_file = generated_header_file[0] + \
        "." + template_name + generated_header_file[1]
    generated_source_file = os.path.splitext(generated_header_file)[0] + ".cpp"

    generated_header_gaurd = replace_all(
        [database.build_directory.lower() + "/", "include/", "inc/", "src/"], "", generated_header_file.lower())
    generated_header_gaurd = make_vaild_identifier(generated_header_gaurd.upper())

    output_directory = os.path.dirname(generated_header_file)
    if not os.path.exists(output_directory):
        try:
            os.makedirs(output_directory)
        except:  # keep from failing when this script is run in parallel
            if not os.path.exists(output_directory):
                raise

    reflection_db = reflection_database(database.source_directory)
    index = clang.cindex.Index.create(True)
    tu = database.parse(index, header_file)
    if tu != None:
        if reflection_db.extract(tu.cursor):
            for template_ext, generated_file in [(".cpp.j2", generated_source_file), (".hpp.j2", generated_header_file)]:
                file_path = template_file_path + template_ext
                if not os.path.exists(file_path):
                    continue
                with open(file_path, "r") as template_file:
                    template = jinja2.Template(template_file.read())

                    with open(generated_file, 'w') as output:
                        output.write(template.render(header_file=header_file, generated_header_file=generated_header_file, generated_source_file=generated_source_file,
                                                     generated_header_gaurd=generated_header_gaurd, database=reflection_db))
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

render_template_parser = sub_parsers.add_parser("template")
render_template_parser.add_argument('-t', '--template-file', type=str,
                                    help='The template file used to generate the metadata.', required=True)

args = parser.parse_args()

database = compile_database(os.path.abspath(args.source_directory),
                            os.path.abspath(args.build_directory))

# reflection_db = reflection_database(database.source_directory)

if args.sub_parser == "template":
    render_metadata_template(database, os.path.abspath(
        args.template_file), os.path.abspath(args.file))
else:
    pass
