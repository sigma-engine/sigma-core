#ifndef SIGMAFIVE_GRAPHICS_OPENGL_BUFFER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_BUFFER_HPP

#include <gl_core_4_0.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            enum class buffer_usage:GLenum {
                stream_draw = gl::STREAM_DRAW,
                stream_read = gl::STREAM_READ,
                stream_copy = gl::STREAM_COPY,
                static_draw = gl::STATIC_DRAW,
                static_read = gl::STATIC_READ,
                static_copy = gl::STATIC_COPY,
                dynamic_draw = gl::DYNAMIC_DRAW,
                dynamic_read = gl::DYNAMIC_READ,
                dynamic_copy = gl::DYNAMIC_COPY
            };
        }
    }
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_BUFFER_HPP