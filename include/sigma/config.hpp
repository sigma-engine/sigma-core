#ifndef SIGMA_CONFIG_HPP
#define SIGMA_CONFIG_HPP

#include <chrono>

using namespace std::literals::chrono_literals;

// clang-format off
#if defined(SIGMA_SHARED)
    #if defined(_WIN32)
        #if defined(SIGMA_EXPORT)
            #define SIGMA_API __declspec(dllexport)
        #else
            #define SIGMA_API __declspec(dllimport)
        #endif // SIGMA_EXPORT
    #elif __GNUC__ >= 4
        #define SIGMA_API __attribute__((visibility("default")))
    #endif
#else
    #define SIGMA_API
#endif // SIGMA_SHARED
// clang-format on

#endif // SIGMA_CONFIG_HPP
