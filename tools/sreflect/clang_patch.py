import ctypes
import clang.cindex


def patch_helper(classtype, name, library_function, args, result):
    f = getattr(clang.cindex.conf.lib, library_function)
    f.argtypes = args
    f.restype = result

    def impl(*args):
        return f(*args)
    setattr(classtype, name, impl)

clang.cindex.TranslationUnit.PARSE_KEEP_GOING = 0x200

patch_helper(clang.cindex.Type, 'get_template_argument_type',
             'clang_Type_getTemplateArgumentAsType',
             [clang.cindex.Type, ctypes.c_uint],
             clang.cindex.Type)

patch_helper(clang.cindex.Type, 'get_num_template_arguments',
             'clang_Type_getNumTemplateArguments',
             [clang.cindex.Type],
             ctypes.c_int)


patch_helper(clang.cindex.SourceLocation, 'is_from_main_file',
             'clang_Location_isFromMainFile',
             [clang.cindex.SourceLocation],
             ctypes.c_int)
