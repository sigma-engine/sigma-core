#ifndef SIGMAFIVE_EDITOR_CONFIG_HPP
#define SIGMAFIVE_EDITOR_CONFIG_HPP

#if defined(_WIN32)
#if defined(editor_core_EXPORT) // inside DLL
#    define EDITOR_API   __declspec(dllexport)
#else // outside DLL
#   define EDITOR_API   __declspec(dllimport)
#endif  // editor_core_EXPORT
#else
#   define EDITOR_API
#endif //_WIN32

#endif //SIGMAFIVE_EDITOR_CONFIG_HPP
