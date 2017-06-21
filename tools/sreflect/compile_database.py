import clang.cindex
import os
import re
import sys


class compile_database:
    def __init__(self, source_directory, build_directory):
        self.source_directory = source_directory
        self.build_directory = build_directory
        self.database = clang.cindex.CompilationDatabase.fromDirectory(build_directory)
        self.source_extensions = ['.cpp', '.cxx', '.cc', '.c', '.m', '.mm']
        self.header_extensions = ['.h', '.hxx', '.hpp', '.hh']

    def parse(self, index, source_file):
        ext = os.path.splitext(source_file)[1][1:]
        arguments = self.__get_compile_arguments(source_file)
        tu = index.parse(source_file, arguments)
        parse_success = True
        safe_to_ignore_error = re.compile(
            r"(.*):(\d+):(\d+):\s+fatal\s+error:\s*'(.*\..*\." + ext + r")'\s+file\s+not\s+found")
        for diagnostic in tu.diagnostics:
            if diagnostic.severity >= 3:
                message = str(diagnostic)
                if not safe_to_ignore_error.match(message):
                    sys.stderr.write("sreflect:\n" + self.source_directory + '/' + message + "\n")
                    parse_success = False
        if parse_success:
            return tu
        return None

    def __is_header(self, source):
        extension = os.path.splitext(source)[1]
        return extension in self.header_extensions

    def __get_best_compile_command(self, source):
        if self.__is_header(source):
            name = os.path.splitext(source)[0]
            basenames = [name, name.replace('include/', 'src/')]
            for basename in basenames:
                for extension in self.source_extensions:
                    replacement_file = basename + extension
                    if os.path.exists(replacement_file):
                        return self.database.getCompileCommands(replacement_file)[0]
            while len(basenames) > 0:
                basenames = [os.path.dirname(p) for p in basenames if len(os.path.basename(p)) > 0]
                for basepath in basenames:
                    for f in os.listdir(basepath):
                        f = os.path.join(basepath, f)
                        ext = os.path.splitext(f)[1]
                        if os.path.isfile(f) and f != source and ext in self.source_extensions:
                            cmd = self.__get_best_compile_command(f)
                            if cmd != None:
                                return cmd
        else:
            return self.database.getCompileCommands(source)[0]

    def __get_compile_arguments(self, source):
        command = self.__get_best_compile_command(source)
        args = [arg for arg in command.arguments]
        args += ['-I/usr/lib64/clang/4.0.0/include']
        args += ['-D__REFLECTION_PARSER__=']
        try:
            i = args.index('-c')
            args = args[:i] + args[i + 2:]

            i = args.index('-o')
            args = args[:i] + args[i + 2:]
        except IndexError:
            pass
        return args
