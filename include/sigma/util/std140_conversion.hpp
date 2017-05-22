#ifndef SIGMA_STD140_CONVERSION_HPP
#define SIGMA_STD140_CONVERSION_HPP

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstddef>
#include <cstring>

#define std140_sizeof(T) sigma::std140::type_traits<T>::size

namespace sigma {
namespace std140 {
    template <class>
    struct type_traits;

    static const constexpr std::ptrdiff_t round_up(std::ptrdiff_t number, std::ptrdiff_t multiple)
    {
        return ((number + multiple - 1) / multiple) * multiple;
    }

    template <class T>
    std::ptrdiff_t to_std140(const T& source, std::uint8_t* output, std::ptrdiff_t& offset)
    {
        return type_traits<T>::convert(source, output, offset);
    }

    template <>
    struct type_traits<float> {
        static const constexpr std::size_t alignment = sizeof(float);
        static const constexpr std::size_t size = sizeof(float);
        static std::ptrdiff_t convert(float source, std::uint8_t* output, std::ptrdiff_t& offset)
        {
            auto N = sizeof(float);
            offset = round_up(offset, alignment);
            std::memcpy(output + offset, &source, sizeof(float));
            return offset + N;
        }
    };

    template <>
    struct type_traits<glm::vec2> {
        static const constexpr std::size_t alignment = sizeof(glm::vec2);
        static const constexpr std::size_t size = sizeof(glm::vec2);
        static std::ptrdiff_t convert(const glm::vec2& source, std::uint8_t* output, std::ptrdiff_t& offset)
        {
            auto N = sizeof(float);
            offset = round_up(offset, alignment);
            std::memcpy(output + offset, glm::value_ptr(source), sizeof(glm::vec2));
            return offset + 2 * N;
        }
    };

    template <>
    struct type_traits<glm::vec3> {
        static const constexpr std::size_t alignment = sizeof(glm::vec4);
        static const constexpr std::size_t size = sizeof(glm::vec4);
        static std::ptrdiff_t convert(const glm::vec3& source, std::uint8_t* output, std::ptrdiff_t& offset)
        {
            auto N = sizeof(float);
            offset = round_up(offset, alignment);
            std::memcpy(output + offset, glm::value_ptr(source), sizeof(glm::vec3));
            return offset + 3 * N;
        }
    };

    template <>
    struct type_traits<glm::vec4> {
        static const constexpr std::size_t alignment = sizeof(glm::vec4);
        static const constexpr std::size_t size = sizeof(glm::vec4);
        static std::ptrdiff_t convert(const glm::vec4& source, std::uint8_t* output, std::ptrdiff_t& offset)
        {
            auto N = sizeof(float);
            offset = round_up(offset, alignment);
            std::memcpy(output + offset, glm::value_ptr(source), sizeof(glm::vec4));
            return offset + 4 * N;
        }
    };

    template <>
    struct type_traits<glm::quat> {
        static const constexpr std::size_t alignment = sizeof(glm::vec4);
        static const constexpr std::size_t size = sizeof(glm::vec4);
        static std::ptrdiff_t convert(const glm::quat& source, std::uint8_t* output, std::ptrdiff_t& offset)
        {
            auto N = sizeof(float);
            offset = round_up(offset, alignment);
            std::memcpy(output + offset, glm::value_ptr(source), sizeof(glm::vec4));
            return offset + 4 * N;
        }
    };

    template <>
    struct type_traits<glm::mat2> {
        static const constexpr std::size_t alignment = sizeof(glm::vec4);
        static const constexpr std::size_t size = 2 * sizeof(glm::vec4);
        static std::ptrdiff_t convert(const glm::mat2& source, std::uint8_t* output, std::ptrdiff_t& offset)
        {
            auto N = sizeof(float);
            offset = round_up(offset, alignment);
            std::memcpy(output + offset, glm::value_ptr(source[0]), sizeof(glm::vec2));
            std::memcpy(output + offset + alignment, glm::value_ptr(source[1]), sizeof(glm::vec2));
            return offset + 2 * 4 * N;
        }
    };

    template <>
    struct type_traits<glm::mat3> {
        static const constexpr std::size_t alignment = sizeof(glm::vec4);
        static const constexpr std::size_t size = 3 * sizeof(glm::vec4);
        static std::ptrdiff_t convert(const glm::mat3& source, std::uint8_t* output, std::ptrdiff_t& offset)
        {
            auto N = sizeof(float);
            offset = round_up(offset, alignment);
            std::memcpy(output + offset, glm::value_ptr(source[0]), sizeof(glm::vec3));
            std::memcpy(output + offset + alignment, glm::value_ptr(source[1]), sizeof(glm::vec3));
            std::memcpy(output + offset + 2 * alignment, glm::value_ptr(source[2]), sizeof(glm::vec3));
            return offset + 3 * 4 * N;
        }
    };

    template <>
    struct type_traits<glm::mat4> {
        static const constexpr std::size_t alignment = sizeof(glm::vec4);
        static const constexpr std::size_t size = 4 * sizeof(glm::vec4);
        static std::ptrdiff_t convert(const glm::mat4& source, std::uint8_t* output, std::ptrdiff_t& offset)
        {
            auto N = sizeof(float);
            offset = round_up(offset, alignment);
            std::memcpy(output + offset, glm::value_ptr(source), sizeof(glm::mat4));
            return offset + 4 * 4 * N;
        }
    };

    template <class T, long int SIZE>
    struct type_traits<T[SIZE]> {
        static const constexpr std::size_t alignment = round_up(type_traits<T>::alignment, type_traits<glm::vec4>::alignment);
        static const constexpr std::size_t size = round_up(type_traits<T>::size * SIZE, alignment);
        static std::ptrdiff_t convert(const T* source, std::uint8_t* output, std::ptrdiff_t& offset)
        {
            offset = round_up(offset, alignment);
            std::ptrdiff_t temp_offset = offset;
            for (long int i = 0; i < SIZE; ++i) {
                temp_offset = to_std140(source[i], output, temp_offset);
                temp_offset = round_up(temp_offset, alignment);
            }
            return round_up(temp_offset, alignment);
        }
    };
}
}

#endif // SIGMA_STD140_CONVERSION_HPP
