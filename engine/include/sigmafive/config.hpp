#ifndef SIGMAFIVE_CONFIG_HPP
#define SIGMAFIVE_CONFIG_HPP

#if defined(ENGINE_EXPORT) // inside DLL
#    define SIGMAFIVE_API   __declspec(dllexport)
#else // outside DLL
#   define SIGMAFIVE_API   __declspec(dllimport)
#endif  // SIGMAFIVE_EXPORT

#endif //SIGMAFIVE_CONFIG_HPP