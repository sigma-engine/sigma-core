#ifndef SIMPLE_GAME_CONFIG_HPP
#define SIMPLE_GAME_CONFIG_HPP

// clang-format off
#if defined(SIMPLE_GAME_SHARED)
    #if defined(_WIN32)
        #if defined(SIMPLE_GAME_EXPORT)
            #define SIMPLE_GAME_API __declspec(dllexport)
        #else
            #define SIMPLE_GAME_API __declspec(dllimport)
        #endif // SIMPLE_GAME_EXPORT
    #elif __GNUC__ >= 4
        #define SIMPLE_GAME_API __attribute__((visibility("default")))
    #endif
#else
    #define SIMPLE_GAME_API
#endif // SIMPLE_GAME_SHARED
// clang-format on

#endif // SIMPLE_GAME_CONFIG_HPP
