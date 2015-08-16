#ifndef SIGMAFIVE_TYPE_INFO_HPP
#define SIGMAFIVE_TYPE_INFO_HPP

#include <sigmafive/math/vec2.hpp>
#include <sigmafive/math/vec3.hpp>
#include <sigmafive/math/vec4.hpp>
#include <sigmafive/math/mat2.hpp>
#include <sigmafive/math/mat3.hpp>
#include <sigmafive/math/mat4.hpp>
#include <sigmafive/math/quaternion.hpp>

namespace sigmafive {

    template<class T,typename U>
    std::ptrdiff_t member_offset(U T::* member) {
        return reinterpret_cast<std::ptrdiff_t >(&(static_cast<T*>(nullptr)->*member));
    }

    //TODO move these into their respective files
    template<typename>
    struct component_info;

    template<typename T>
    struct component_info<vec2_t<T>> {
        static constexpr const int number_of_components = 2;
        typedef T component_type;
    };

    template<typename T>
    struct component_info<vec3_t<T>> {
        static constexpr const int number_of_components = 3;
        typedef T component_type;
    };

    template<typename T>
    struct component_info<vec4_t<T>> {
        static constexpr const int number_of_components = 4;
        typedef T component_type;
    };

    template<typename T>
    struct component_info<quaternion_t<T>> {
        static constexpr const int number_of_components = 4;
        typedef T component_type;
    };

    template<typename T>
    struct component_info<mat2x2_t<T>> {
        static constexpr const int number_of_components = 2;
        typedef T component_type;
    };

    template<typename T>
    struct component_info<mat3x3_t<T>> {
        static constexpr const int number_of_components = 3;
        typedef T component_type;
    };

    template<typename T>
    struct component_info<mat4x4_t<T>> {
        static constexpr const int number_of_components = 4;
        typedef T component_type;
    };
}

//http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3911
template< class... > struct voider { using type = void; };
template< class... T0toN > using void_t = typename voider<T0toN...>::type;

#endif //SIGMAFIVE_TYPE_INFO_HPP
