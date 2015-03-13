#ifndef SIGMAFIVE_TYPE_INFO_HPP
#define SIGMAFIVE_TYPE_INFO_HPP

#include <math/vec2.hpp>
#include <math/vec3.hpp>
#include <math/vec4.hpp>
#include <math/mat2.hpp>
#include <math/mat3.hpp>
#include <math/mat4.hpp>
#include <math/quaternion.hpp>

namespace sigmafive {

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
    struct component_info<quad_t<T>> {
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

#endif //SIGMAFIVE_TYPE_INFO_HPP
