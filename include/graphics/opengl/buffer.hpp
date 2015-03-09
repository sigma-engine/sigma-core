#ifndef SIGMAFIVE_GRAPHICS_OPENGL_BUFFER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_BUFFER_HPP

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            enum buffer_usage {
                stream_draw,
                stream_read,
                stream_copy,
                static_draw,
                static_read,
                static_copy,
                dynamic_draw,
                dynamic_read,
                dynamic_copy
            };
        }
    }
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_BUFFER_HPP