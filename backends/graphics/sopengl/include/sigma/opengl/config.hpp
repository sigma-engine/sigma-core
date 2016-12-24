#ifndef SIGMA_FIVE_OPENGL_CONFIG_HPP
#define SIGMA_FIVE_OPENGL_CONFIG_HPP

#if defined _WIN32 || defined __CYGWIN__
#if defined(SIGMA_FIVE_OPENGL_EXPORT)
#define SIGMA_FIVE_OPENGL_API __declspec(dllexport)
#else
#define SIGMA_FIVE_OPENGL_API __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define SIGMA_FIVE_OPENGL_API __attribute__((visibility("default")))
#else
#define SIGMA_FIVE_OPENGL_API
#endif
#endif //_WIN32

#endif // SIGMA_FIVE_OPENGL_CONFIG_HPP
