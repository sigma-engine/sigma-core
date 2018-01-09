#ifndef SIGMA_GRAPHICS_OPENGL_UTIL_HPP
#define SIGMA_GRAPHICS_OPENGL_UTIL_HPP

#include <sigma/config.hpp>

#include <glad/glad.h>

namespace sigma {
namespace opengl {
    enum data_types {
        bool_type = GL_BOOL,

        byte_type = GL_BYTE,
        unsigned_byte_type = GL_UNSIGNED_BYTE,

        short_type = GL_SHORT,
        unsigned_short_type = GL_UNSIGNED_SHORT,

        int_type = GL_INT,
        unsigned_int_type = GL_UNSIGNED_INT,

        float_type = GL_FLOAT,

        double_type = GL_DOUBLE,
    };

    template <typename T>
    struct enum_type;

    template <>
    struct enum_type<bool> {
        static const data_types value = bool_type;
    };

    template <>
    struct enum_type<GLbyte> {
        static const data_types value = byte_type;
    };

    template <>
    struct enum_type<GLubyte> {
        static const data_types value = unsigned_byte_type;
    };

    template <>
    struct enum_type<GLshort> {
        static const data_types value = short_type;
    };

    template <>
    struct enum_type<GLushort> {
        static const data_types value = unsigned_short_type;
    };

    template <>
    struct enum_type<GLint> {
        static const data_types value = int_type;
    };

    template <>
    struct enum_type<GLuint> {
        static const data_types value = unsigned_int_type;
    };

    template <>
    struct enum_type<GLfloat> {
        static const data_types value = float_type;
    };

    template <>
    struct enum_type<GLdouble> {
        static const data_types value = double_type;
    };

    template <typename T>
    T calculate_mipmap_levels(T x_size, T y_size)
    {
        T count = 0;
        while ((x_size /= 2) > 0 && (y_size /= 2) > 0)
            ++count;
        return count;
    }
}
}

#endif // SIGMA_GRAPHICS_OPENGL_UTIL_HPP
