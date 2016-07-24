#ifndef SIGMA_FIVE_TYPE_INFO_HPP
#define SIGMA_FIVE_TYPE_INFO_HPP

// TODO move to sigmafive/-typeinfo or something like that

#include <glm/gtc/quaternion.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace sigmafive {
namespace opengl {
    template <class T, typename U>
    std::ptrdiff_t member_offset(U T::*member)
    {
        return reinterpret_cast<std::ptrdiff_t>(
            &(static_cast<T*>(nullptr)->*member));
    }

    // TODO move these into their respective files
    template <typename T>
    struct component_info;

    template <typename T, glm::precision P>
    struct component_info<glm::tvec2<T, P> > {
        static constexpr const int number_of_components = 2;
        typedef T component_type;
    };

    template <typename T, glm::precision P>
    struct component_info<glm::tvec3<T, P> > {
        static constexpr const int number_of_components = 3;
        typedef T component_type;
    };

    template <typename T, glm::precision P>
    struct component_info<glm::tvec4<T, P> > {
        static constexpr const int number_of_components = 4;
        typedef T component_type;
    };

    template <typename T, glm::precision P>
    struct component_info<glm::tquat<T, P> > {
        static constexpr const int number_of_components = 4;
        typedef T component_type;
    };

    template <typename T, glm::precision P>
    struct component_info<glm::tmat2x2<T, P> > {
        static constexpr const int number_of_components = 2;
        typedef T component_type;
    };

    template <typename T, glm::precision P>
    struct component_info<glm::tmat3x3<T, P> > {
        static constexpr const int number_of_components = 3;
        typedef T component_type;
    };

    template <typename T, glm::precision P>
    struct component_info<glm::tmat4x4<T, P> > {
        static constexpr const int number_of_components = 4;
        typedef T component_type;
    };
}
}

#endif // SIGMA_FIVE_TYPE_INFO_HPP
