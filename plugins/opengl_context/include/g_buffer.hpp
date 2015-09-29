#ifndef SIGMAFIVE_GRAPHICS_OPENGL_G_BUFFER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_G_BUFFER_HPP

#include <cppbr/opengl/texture_2d.hpp>
#include <cppbr/opengl/frame_buffer.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            class g_buffer {
            public:
                g_buffer(uint2 size);

                uint2 size_;
                cppbr::opengl::frame_buffer frame_buffer_;
                cppbr::opengl::texture_2d position_texture_;
                cppbr::opengl::texture_2d diffuse_texture_;
                cppbr::opengl::texture_2d normal_texture_;
                cppbr::opengl::texture_2d texcoord_texture_;
                cppbr::opengl::texture_2d depth_texture_;
            };
        }
    }
}


#endif //SIGMAFIVE_GRAPHICS_OPENGL_G_BUFFER_HPP
