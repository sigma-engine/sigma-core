#ifndef SIGMAFIVE_CONFIG_HPP
#define SIGMAFIVE_CONFIG_HPP

#if defined _WIN32 || defined __CYGWIN__
#if defined(SIGMA_FIVE_ENGINE_EXPORT)
#define SIGMAFIVE_API __declspec(dllexport)
#else
#define SIGMAFIVE_API __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define SIGMAFIVE_API __attribute__((visibility("default")))
#else
#define SIGMAFIVE_API
#endif
#endif //_WIN32

#endif // SIGMAFIVE_CONFIG_HPP
