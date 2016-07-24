#ifndef sigma_CONFIG_HPP
#define sigma_CONFIG_HPP

#if defined _WIN32 || defined __CYGWIN__
#if defined(SIGMA_ENGINE_EXPORT)
#define sigma_API __declspec(dllexport)
#else
#define sigma_API __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define sigma_API __attribute__((visibility("default")))
#else
#define sigma_API
#endif
#endif //_WIN32

#endif // sigma_CONFIG_HPP
