#include <frame_buffer.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            void frame_buffer::bind_default(frame_buffer_target target) {
                gl::BindFramebuffer(GLenum(target),0);
            }

            frame_buffer::frame_buffer() {
                gl::GenFramebuffers(1,&gl_id_);
            }

            frame_buffer::~frame_buffer() {
                gl::DeleteFramebuffers(1,&gl_id_);
            }

            bool frame_buffer::good() {
                bind();
                //TODO it would be better to use glCheckNamedFramebufferStatus from 4.5
                return gl::CheckFramebufferStatus(gl::FRAMEBUFFER) == gl::FRAMEBUFFER_COMPLETE;
            }

            void frame_buffer::bind(frame_buffer_target target) {
                gl::BindFramebuffer(GLenum(target),gl_id_);
            }

            void frame_buffer::attach_texture(frame_buffer_target target, frame_buffer_attachment attachment,
                                              texture_2d_target textarget,
                                              texture_2d &texture,
                                              GLint level) {
                //TODO it would be better to use glNamedFramebufferTexture from 4.5
                bind(target);
                gl::FramebufferTexture2D(GLenum(target),GLenum(attachment),GLenum(textarget),texture,level);
            }
        }
    }
}
