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

    static std::ptrdiff_t round_up(std::ptrdiff_t number, std::ptrdiff_t multiple)
    {
        return ((number + multiple - 1) / multiple) * multiple;
    }

    // static std::ptrdiff_t to_std140(bool source,std::uint8_t* output, std::ptrdiff_t &offset)
    // {
    //     output = source;
    // }
    //
    // static std::ptrdiff_t to_std140(int source,std::uint8_t* output, std::ptrdiff_t &offset)
    // {
    //     output = source;
    // }
    //
    // static std::ptrdiff_t to_std140(unsigned long int source,std::uint8_t* output, std::ptrdiff_t &offset)
    // {
    //     output = (Json::UInt64)source;
    // }

    static std::ptrdiff_t to_std140(float source, std::uint8_t* output, std::ptrdiff_t& offset)
    {
        auto N = sizeof(float);
        auto alignment = N;
        offset = round_up(offset, alignment);
        std::memcpy(output + offset, &source, sizeof(float));
        return offset + N;
    }

    static std::ptrdiff_t to_std140(const glm::vec2& source, std::uint8_t* output, std::ptrdiff_t& offset)
    {
        auto N = sizeof(float);
        auto alignment = 2 * N;
        offset = round_up(offset, alignment);
        std::memcpy(output + offset, glm::value_ptr(source), sizeof(glm::vec2));
        return offset + 2 * N;
    }

    static std::ptrdiff_t to_std140(const glm::vec3& source, std::uint8_t* output, std::ptrdiff_t& offset)
    {
        auto N = sizeof(float);
        auto alignment = 4 * N;
        offset = round_up(offset, alignment);
        std::memcpy(output + offset, glm::value_ptr(source), sizeof(glm::vec3));
        return offset + 3 * N;
    }

    static std::ptrdiff_t to_std140(const glm::vec4& source, std::uint8_t* output, std::ptrdiff_t& offset)
    {
        auto N = sizeof(float);
        auto alignment = 4 * N;
        offset = round_up(offset, alignment);
        std::memcpy(output + offset, glm::value_ptr(source), sizeof(glm::vec4));
        return offset + 4 * N;
    }

    static std::ptrdiff_t to_std140(const glm::quat& source, std::uint8_t* output, std::ptrdiff_t& offset)
    {
        auto N = sizeof(float);
        auto alignment = 4 * N;
        offset = round_up(offset, alignment);
        std::memcpy(output + offset, glm::value_ptr(source), sizeof(glm::vec4));
        return offset + 4 * N;
    }

    static std::ptrdiff_t to_std140(const glm::mat2& source, std::uint8_t* output, std::ptrdiff_t& offset)
    {
        auto N = sizeof(float);
        auto alignment = 4 * N;
        offset = round_up(offset, alignment);
        std::memcpy(output + offset, glm::value_ptr(source[0]), sizeof(glm::vec2));
        std::memcpy(output + offset + alignment, glm::value_ptr(source[1]), sizeof(glm::vec2));
        return offset + 2 * 4 * N;
    }

    static std::ptrdiff_t to_std140(const glm::mat3& source, std::uint8_t* output, std::ptrdiff_t& offset)
    {
        auto N = sizeof(float);
        auto alignment = 4 * N;
        offset = round_up(offset, alignment);
        std::memcpy(output + offset, glm::value_ptr(source[0]), sizeof(glm::vec3));
        std::memcpy(output + offset + alignment, glm::value_ptr(source[1]), sizeof(glm::vec3));
        std::memcpy(output + offset + 2 * alignment, glm::value_ptr(source[2]), sizeof(glm::vec3));
        return offset + 3 * 4 * N;
    }

    static std::ptrdiff_t to_std140(const glm::mat4& source, std::uint8_t* output, std::ptrdiff_t& offset)
    {
        auto N = sizeof(float);
        auto alignment = 4 * N;
        offset = round_up(offset, alignment);
        std::memcpy(output + offset, glm::value_ptr(source), sizeof(glm::mat4));
        return offset + 4 * 4 * N;
    }
}
}

#endif // SIGMA_STD140_CONVERSION_HPP
