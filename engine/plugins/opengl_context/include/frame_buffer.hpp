#ifndef SIGMAFIVE_GRAPHICS_OPENGL_FRAME_BUFFER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_FRAME_BUFFER_HPP

#include <gl_core_4_0.hpp>
#include <texture_2d.hpp>

#include <vector>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            enum class frame_buffer_target : GLenum {
                read = gl::READ_FRAMEBUFFER,
                draw = gl::DRAW_FRAMEBUFFER,
                both = gl::FRAMEBUFFER
            };

            enum class frame_buffer_attachment : GLenum {
                color0 = gl::COLOR_ATTACHMENT0,
                color1 = gl::COLOR_ATTACHMENT1,
                color2 = gl::COLOR_ATTACHMENT2,
                color3 = gl::COLOR_ATTACHMENT3,
                color4 = gl::COLOR_ATTACHMENT4,
                color5 = gl::COLOR_ATTACHMENT5,
                color6 = gl::COLOR_ATTACHMENT6,
                color7 = gl::COLOR_ATTACHMENT7,
                color8 = gl::COLOR_ATTACHMENT8,
                color9 = gl::COLOR_ATTACHMENT9,
                color10 = gl::COLOR_ATTACHMENT10,
                color11 = gl::COLOR_ATTACHMENT11,
                color12 = gl::COLOR_ATTACHMENT12,
                color13 = gl::COLOR_ATTACHMENT13,
                color14 = gl::COLOR_ATTACHMENT14,
                color15 = gl::COLOR_ATTACHMENT15,

                depth = gl::DEPTH_ATTACHMENT,
                stencil = gl::STENCIL_ATTACHMENT
            };

            class frame_buffer {
            public:
                static void bind_default(frame_buffer_target target = frame_buffer_target::both);

                frame_buffer();

                frame_buffer(const frame_buffer &) = delete;

                frame_buffer &operator=(const frame_buffer &) = delete;

                ~frame_buffer();

                bool good();

                void bind(frame_buffer_target target = frame_buffer_target::both);

                void attach_texture(frame_buffer_target target, frame_buffer_attachment attachment,
                                    texture_2d_target textarget,
                                    texture_2d &texture,
                                    GLint level = 0);

                void set_draw_buffers(std::vector<frame_buffer_attachment> draw_buffers) {
                    bind(frame_buffer_target::draw); //TODO it would be better to use glNamedFramebufferDrawBuffers from 4.5
                    gl::DrawBuffers(GLsizei(draw_buffers.size()),(GLenum*)draw_buffers.data());
                }
            private:
                GLuint gl_id_;
            };
        }
    }
}


#endif //SIGMAFIVE_GRAPHICS_OPENGL_FRAME_BUFFER_HPP
