#include <g_buffer.hpp>
#include <stdexcept>
#include <iostream>

using namespace cppbr::opengl;

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            g_buffer::g_buffer(uint2 size)
                    : position_texture_(texture_format::rgb32f, size, pixel_format::rgb, pixel_type::float_),
                      diffuse_texture_(texture_format::rgb32f, size, pixel_format::rgb, pixel_type::float_),
                      normal_texture_(texture_format::rgb32f, size, pixel_format::rgb, pixel_type::float_),
                      texcoord_texture_(texture_format::rgb32f, size, pixel_format::rgb, pixel_type::float_),
                      depth_texture_(texture_format::depth_component32f,size,pixel_format::depth_component,pixel_type::float_) {

                //world-space position texture
                frame_buffer_.attach_texture(frame_buffer_target::draw, frame_buffer_attachment::color0,
                                             texture_2d_target::texture_2d, position_texture_);

                //diffuse texture
                frame_buffer_.attach_texture(frame_buffer_target::draw, frame_buffer_attachment::color1,
                                             texture_2d_target::texture_2d, diffuse_texture_);

                //world-space normal texture
                frame_buffer_.attach_texture(frame_buffer_target::draw, frame_buffer_attachment::color2,
                                             texture_2d_target::texture_2d, normal_texture_);

                //uv texture coordinates texture
                frame_buffer_.attach_texture(frame_buffer_target::draw, frame_buffer_attachment::color3,
                                             texture_2d_target::texture_2d, texcoord_texture_);

                //depth texture
                frame_buffer_.attach_texture(frame_buffer_target::draw, frame_buffer_attachment::depth,
                                             texture_2d_target::texture_2d, depth_texture_);

                //setup the attachments that will be drawn to.
                frame_buffer_.set_draw_buffers({frame_buffer_attachment::color0,
                                                frame_buffer_attachment::color1,
                                                frame_buffer_attachment::color2,
                                                frame_buffer_attachment::color3});

                if(!frame_buffer_.good())
                    throw std::runtime_error("frame buffer has an error");

                frame_buffer::bind_default(frame_buffer_target::draw);
            }
        }
    }
}
