#ifndef SIGMAFIVE_GRAPHICS_OPENGL_UTIL_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_UTIL_HPP

#include <sigmafive/graphics/opengl/gl_core_4_0.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            enum data_types {
                bool_type = gl::BOOL,

                byte_type = gl::BYTE,
                unsigned_byte_type = gl::UNSIGNED_BYTE,

                short_type = gl::SHORT,
                unsigned_short_type = gl::UNSIGNED_SHORT,

                int_type = gl::INT,
                unsigned_int_type = gl::UNSIGNED_INT,

                float_type = gl::FLOAT,

                double_type = gl::DOUBLE,
            };

            template<typename T>
            struct enum_type;

            template<>
            struct enum_type<bool> {
                static const data_types value = bool_type;
            };

            template<>
            struct enum_type<GLbyte> {
                static const data_types value = byte_type;
            };

            template<>
            struct enum_type<GLubyte> {
                static const data_types value = unsigned_byte_type;
            };

            template<>
            struct enum_type<GLshort> {
                static const data_types value = short_type;
            };

            template<>
            struct enum_type<GLushort> {
                static const data_types value = unsigned_short_type;
            };

            template<>
            struct enum_type<GLint> {
                static const data_types value = int_type;
            };

            template<>
            struct enum_type<GLuint> {
                static const data_types value = unsigned_int_type;
            };

            template<>
            struct enum_type<GLfloat> {
                static const data_types value = float_type;
            };

            template<>
            struct enum_type<GLdouble> {
                static const data_types value = double_type;
            };
        }
    }
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_UTIL_HPP