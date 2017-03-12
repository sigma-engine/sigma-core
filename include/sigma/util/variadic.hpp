#ifndef SIGMA_UTIL_VARIADIC_HPP
#define SIGMA_UTIL_VARIADIC_HPP

#include <type_traits>

namespace sigma {
template <typename...>
struct index;

template <typename T, typename... Rest>
struct index<T, T, Rest...> : std::integral_constant<std::size_t, 0> {
};

template <typename T, typename Head, typename... Rest>
struct index<T, Head, Rest...> : std::integral_constant<std::size_t, 1 + index<T, Rest...>::value> {
};
}

#endif // SIGMA_UTIL_VARIADIC_HPP
