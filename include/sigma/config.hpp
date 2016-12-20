#ifndef SIGMA_CONFIG_HPP
#define SIGMA_CONFIG_HPP

#include <chrono>

using namespace std::literals::chrono_literals;

#ifdef SIGMA_SHARED
#if defined _WIN32 || defined __CYGWIN__
#if defined(SIGMA_EXPORT)
#define SIGMA_API __declspec(dllexport)
#else
#define SIGMA_API __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define SIGMA_API __attribute__((visibility("default")))
#else
#define SIGMA_API
#endif
#endif //_WIN32
#else
#define SIGMA_API
#endif // SIGMA_SHARED

#endif // SIGMA_CONFIG_HPP
