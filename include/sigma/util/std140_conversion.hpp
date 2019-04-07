#ifndef SIGMA_STD140_HPP
#define SIGMA_STD140_HPP

#include <sigma/util/numeric.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <algorithm>
#include <array>
#include <cstddef>

#define std140_sizeof(T) sizeof(T) // sigma::std140::detial::type_traits<T>::size()

namespace sigma {
namespace std140 {
    namespace detial {
        template <typename>
        struct type_traits;
    }

    template <typename T>
    static constexpr std::size_t alignment()
    {
        return detial::type_traits<T>::alignment();
    }

    template <typename T>
    static constexpr std::size_t size()
    {
        return detial::type_traits<T>::size();
    }

    template <typename T>
    static void to_std140(const T& source, std::uint8_t* dest)
    {
        memcpy(dest, &source, sizeof(T));
        // detial::type_traits<T>::write(source, dest, 0);
    }

    namespace detial {
        template <class T>
        struct type_traits {
        };

        template <>
        struct type_traits<float> {
            static constexpr std::size_t alignment()
            {
                return sizeof(float);
            }

            static constexpr std::size_t size()
            {
                return sizeof(float);
            }

            static std::size_t write(const float& value, std::uint8_t* buffer, std::size_t offset)
            {
                const std::uint8_t* data = (const std::uint8_t*)&value;
                offset = numeric::round_up(offset, alignment());
                std::copy(data, data + sizeof(float), buffer + offset);
                offset += size();
                return offset;
            }
        };

        template <>
        struct type_traits<double> {
            static constexpr std::size_t alignment()
            {
                return sizeof(double);
            }

            static constexpr std::size_t size()
            {
                return sizeof(double);
            }

            static std::size_t write(const double& value, std::uint8_t* buffer, std::size_t offset)
            {
                const std::uint8_t* data = (const std::uint8_t*)&value;
                offset = numeric::round_up(offset, alignment());
                std::copy(data, data + sizeof(double), buffer + offset);
                offset += size();
                return offset;
            }
        };

        template <>
        struct type_traits<bool> {
            static constexpr std::size_t alignment()
            {
                return type_traits<float>::alignment();
            }

            static constexpr std::size_t size()
            {
                return type_traits<float>::size();
            }

            static std::size_t write(const bool& value, std::uint8_t* buffer, std::size_t offset)
            {
                const std::uint8_t* data = (const std::uint8_t*)&value;
                offset = numeric::round_up(offset, alignment());
                std::copy(data, data + sizeof(bool), buffer + offset);
                offset += size();
                return offset;
            }
        };

        template <>
        struct type_traits<int> {
            static constexpr std::size_t alignment()
            {
                return type_traits<float>::alignment();
            }

            static constexpr std::size_t size()
            {
                return type_traits<float>::size();
            }

            static std::size_t write(const int& value, std::uint8_t* buffer, std::size_t offset)
            {
                const std::uint8_t* data = (const std::uint8_t*)&value;
                offset = numeric::round_up(offset, alignment());
                std::copy(data, data + sizeof(int), buffer + offset);
                offset += size();
                return offset;
            }
        };

        template <>
        struct type_traits<unsigned int> {
            static constexpr std::size_t alignment()
            {
                return type_traits<float>::alignment();
            }

            static constexpr std::size_t size()
            {
                return type_traits<float>::size();
            }

            static std::size_t write(const unsigned int& value, std::uint8_t* buffer, std::size_t offset)
            {
                const std::uint8_t* data = (const std::uint8_t*)&value;
                offset = numeric::round_up(offset, alignment());
                std::copy(data, data + sizeof(unsigned int), buffer + offset);
                offset += size();
                return offset;
            }
        };

        template <typename T>
        struct type_traits<glm::tvec2<T>> {
            static constexpr std::size_t alignment()
            {
                return 2 * type_traits<T>::alignment();
            }

            static constexpr std::size_t size()
            {
                return 2 * type_traits<T>::size();
            }

            static std::size_t write(const glm::tvec2<T>& value, std::uint8_t* buffer, std::size_t offset)
            {
                const std::uint8_t* data = (const std::uint8_t*)glm::value_ptr(value);
                offset = numeric::round_up(offset, alignment());
                std::copy(data, data + sizeof(glm::tvec2<T>), buffer + offset);
                offset += size();
                return offset;
            }
        };

        template <typename T>
        struct type_traits<glm::tvec3<T>> {
            static constexpr std::size_t alignment()
            {
                return 4 * type_traits<T>::alignment();
            }

            static constexpr std::size_t size()
            {
                return 3 * type_traits<T>::size();
            }

            static std::size_t write(const glm::tvec3<T>& value, std::uint8_t* buffer, std::size_t offset)
            {
                const std::uint8_t* data = (const std::uint8_t*)glm::value_ptr(value);
                offset = numeric::round_up(offset, alignment());
                std::copy(data, data + sizeof(glm::tvec3<T>), buffer + offset);
                offset += size();
                return offset;
            }
        };

        template <typename T>
        struct type_traits<glm::tvec4<T>> {
            static constexpr std::size_t alignment()
            {
                return 4 * type_traits<T>::alignment();
            }

            static constexpr std::size_t size()
            {
                return 4 * type_traits<T>::size();
            }

            static std::size_t write(const glm::tvec4<T>& value, std::uint8_t* buffer, std::size_t offset)
            {
                const std::uint8_t* data = (const std::uint8_t*)glm::value_ptr(value);
                offset = numeric::round_up(offset, alignment());
                std::copy(data, data + sizeof(glm::tvec4<T>), buffer + offset);
                offset += size();
                return offset;
            }
        };

        template <typename T>
        struct type_traits<glm::tmat2x2<T>> {
            static constexpr std::size_t alignment()
            {
                return type_traits<glm::tvec4<T>>::alignment();
            }

            static constexpr std::size_t size()
            {
                return 2 * type_traits<glm::tvec4<T>>::size();
            }

            static std::size_t write(const glm::tmat2x2<T>& value, std::uint8_t* buffer, std::size_t offset)
            {
                // TODO investigate this
                offset = numeric::round_up(offset, alignment());
                for (unsigned int i = 0; i < 2; ++i) {
                    const std::uint8_t* data = (const std::uint8_t*)glm::value_ptr(value[i]);
                    std::copy(data, data + 2 * sizeof(T), buffer + offset);
                    offset += alignment();
                }
                return offset;
            }
        };

        template <typename T>
        struct type_traits<glm::tmat3x3<T>> {
            static constexpr std::size_t alignment()
            {
                return type_traits<glm::tvec4<T>>::alignment();
            }

            static constexpr std::size_t size()
            {
                return 3 * type_traits<glm::tvec4<T>>::size();
            }

            static std::size_t write(const glm::tmat3x3<T>& value, std::uint8_t* buffer, std::size_t offset)
            {
                // TODO investigate this
                offset = numeric::round_up(offset, alignment());
                for (unsigned int i = 0; i < 3; ++i) {
                    const std::uint8_t* data = (const std::uint8_t*)glm::value_ptr(value[i]);
                    std::copy(data, data + 3 * sizeof(T), buffer + offset);
                    offset += alignment();
                }
                return offset;
            }
        };

        template <typename T>
        struct type_traits<glm::tmat4x4<T>> {
            static constexpr std::size_t alignment()
            {
                return type_traits<glm::tvec4<T>>::alignment();
            }

            static constexpr std::size_t size()
            {
                return 4 * type_traits<glm::tvec4<T>>::size();
            }

            static std::size_t write(const glm::tmat4x4<T>& value, std::uint8_t* buffer, std::size_t offset)
            {
                // TODO investigate this
                offset = numeric::round_up(offset, alignment());
                for (unsigned int i = 0; i < 4; ++i) {
                    const std::uint8_t* data = (const std::uint8_t*)glm::value_ptr(value[i]);
                    std::copy(data, data + 4 * sizeof(T), buffer + offset);
                    offset += alignment();
                }
                return offset;
            }
        };

        template <typename T, std::size_t N>
        struct type_traits<std::array<T, N>> {
            static constexpr std::size_t alignment()
            {
                return numeric::round_up(type_traits<T>::alignment(), type_traits<glm::vec4>::alignment());
            }

            static constexpr std::size_t size()
            {
                return N * numeric::round_up(type_traits<T>::size(), type_traits<glm::vec4>::alignment());
            }

            static std::size_t write(const std::array<T, N>& value, std::uint8_t* buffer, std::size_t offset)
            {
                // TODO investigate this.
                offset = numeric::round_up(offset, alignment());
                for (unsigned int i = 0; i < N; ++i) {
                    // TODO really investigate this, especially when data is more complicated then basic types.
                    offset = type_traits<T>::write(value[i], buffer, offset);
                    offset = numeric::round_up(offset, alignment());
                }
                return offset;
            }
        };
    }
}
}

#endif // SIGMA_STD140_HPP
