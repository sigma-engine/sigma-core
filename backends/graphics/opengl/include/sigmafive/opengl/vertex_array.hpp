#ifndef SIGMA_FIVE_OPENGL_VERTEX_ARRAY_HPP
#define SIGMA_FIVE_OPENGL_VERTEX_ARRAY_HPP

#include <cstdint>

#include <sigmafive/opengl/index_buffer.hpp>
#include <sigmafive/opengl/type_info.hpp>
#include <sigmafive/opengl/util.hpp>
#include <sigmafive/opengl/vertex_buffer.hpp>

namespace sigmafive {
namespace opengl {
    enum class primitive_type : GLenum {
        points = GL_POINTS,
        line_strip = GL_LINE_STRIP,
        line_loop = GL_LINE_LOOP,
        lines = GL_LINES,
        triangle_strip = GL_TRIANGLE_STRIP,
        triangle_fan = GL_TRIANGLE_FAN,
        triangles = GL_TRIANGLES
    };

    class SIGMA_FIVE_OPENGL_API vertex_array {
    public:
        vertex_array();

        ~vertex_array();

        template <typename T>
        void bind(const vertex_buffer<T>& buffer, unsigned int index,
            bool normalize = false) const
        {
            typedef component_info<T> info;
            bind(buffer, index, info::number_of_components, enum_type<typename info::component_type>::value, normalize, nullptr);
        }

        template <typename T, typename E>
        void bind(const vertex_buffer<T>& buffer, unsigned int index,
            E T::*offset_pointer, bool normalize = false) const
        {
            typedef component_info<E> info;
            bind(buffer, index, info::number_of_components, enum_type<typename info::component_type>::value, normalize, (const void*)member_offset(offset_pointer));
        }

        void draw(primitive_type mode, std::size_t first, std::size_t count) const;

        void draw(primitive_type mode, GLuint index_buffer, std::size_t first, std::size_t count) const;

        std::uint32_t gl_object_;

    private:
        void bind(const vertex_buffer_base& buffer, unsigned int index,
            unsigned int number_of_components, data_types component_type,
            bool normalize, const void* offset) const;
    };
}
}

#endif // SIGMA_FIVE_OPENGL_VERTEX_ARRAY_HPP
