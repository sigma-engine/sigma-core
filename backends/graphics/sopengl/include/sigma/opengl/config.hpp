#ifndef SIGMA_ENGINE_OPENGL_CONFIG_HPP
#define SIGMA_ENGINE_OPENGL_CONFIG_HPP

// clang-format off
#if defined(SIGMA_SOPENGL_SHARED)
    #if defined(_WIN32)
        #if defined(SIGMA_SOPENGL_EXPORT)
            #define SIGMA_SOPENGL_API __declspec(dllexport)
        #else
            #define SIGMA_SOPENGL_API __declspec(dllimport)
        #endif // SIGMA_SOPENGL_EXPORT
    #elif __GNUC__ >= 4
        #define SIGMA_SOPENGL_API __attribute__((visibility("default")))
    #endif
#else
    #define SIGMA_SOPENGL_API
#endif // SIGMA_SHARED
// clang-format on

#endif // SIGMA_ENGINE_OPENGL_CONFIG_HPP
