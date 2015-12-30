#ifndef SIGMAFIVE_CONFIG_HPP
#define SIGMAFIVE_CONFIG_HPP

#if defined(_WIN32)
#if defined(SIGMA_FIVE_ENGINE_EXPORT) // inside DLL
#    define SIGMAFIVE_API   __declspec(dllexport)
#else // outside DLL
#   define SIGMAFIVE_API   __declspec(dllimport)
#endif  // engine_EXPORT
#else
#   define SIGMAFIVE_API
#endif //_WIN32

#endif //SIGMAFIVE_CONFIG_HPP
