#ifndef SIGMA_ENGINE_OPENGL_UTIL_HPP
#define SIGMA_ENGINE_OPENGL_UTIL_HPP

#include <sigma/opengl/gl_core_4_0.h>

#include <sstream>
#include <string>

#define USE_GL_CHECK
#ifdef USE_GL_CHECK
#define GL_CHECK(x)                                                                            \
    {                                                                                          \
        x;                                                                                     \
        auto gl_error_code_check = glGetError();                                               \
        if (gl_error_code_check != GL_NO_ERROR) {                                              \
            std::stringstream stream;                                                          \
            stream << __FILE__ << ":" << __LINE__ << " 0x" << std::hex << gl_error_code_check; \
            throw std::runtime_error(stream.str());                                            \
        }                                                                                      \
    }
#else
#define GL_CHECK(x) x
#endif // USE_GL_CHECK
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
}
}

#endif // SIGMA_ENGINE_OPENGL_UTIL_HPP
