#ifndef SIGMA_FIVE_OPENGL_FRAME_BUFFER_HPP
#define SIGMA_FIVE_OPENGL_FRAME_BUFFER_HPP

#include <vector>

#include <sigma/opengl/gl_core_4_0.h>
#include <sigma/opengl/texture_2d.hpp>

namespace sigma {
namespace opengl {
    /*enum class frame_buffer_target : GLenum {
    read = GL_READ_FRAMEBUFFER,
    draw = GL_DRAW_FRAMEBUFFER,
    both = GL_FRAMEBUFFER
};

enum class frame_buffer_attachment : GLenum {
    color0 = GL_COLOR_ATTACHMENT0,
    color1 = GL_COLOR_ATTACHMENT1,
    color2 = GL_COLOR_ATTACHMENT2,
    color3 = GL_COLOR_ATTACHMENT3,
    color4 = GL_COLOR_ATTACHMENT4,
    color5 = GL_COLOR_ATTACHMENT5,
    color6 = GL_COLOR_ATTACHMENT6,
    color7 = GL_COLOR_ATTACHMENT7,
    color8 = GL_COLOR_ATTACHMENT8,
    color9 = GL_COLOR_ATTACHMENT9,
    color10 = GL_COLOR_ATTACHMENT10,
    color11 = GL_COLOR_ATTACHMENT11,
    color12 = GL_COLOR_ATTACHMENT12,
    color13 = GL_COLOR_ATTACHMENT13,
    color14 = GL_COLOR_ATTACHMENT14,
    color15 = GL_COLOR_ATTACHMENT15,

    depth = GL_DEPTH_ATTACHMENT,
    stencil = GL_STENCIL_ATTACHMENT
};

class SIGMA_FIVE_OPENGL_API frame_buffer {
public:
    static void bind_default(frame_buffer_target target =
frame_buffer_target::both);

    frame_buffer();

    frame_buffer(const frame_buffer&) = delete;

    frame_buffer& operator=(const frame_buffer&) = delete;

    ~frame_buffer();

    bool good();

    void bind(frame_buffer_target target = frame_buffer_target::both);

    void attach_texture(frame_buffer_target target, frame_buffer_attachment
attachment,
        texture_2d_target textarget,
        texture_2d& texture,
        GLint level = 0);

    void set_draw_buffers(std::vector<frame_buffer_attachment> draw_buffers)
    {
        bind(frame_buffer_target::draw); //TODO it would be better to use
glNamedFramebufferDrawBuffers from 4.5
        glDrawBuffers(GLsizei(draw_buffers.size()),
(GLenum*)draw_buffers.data());
    }

private:
    GLuint gl_id_;
};*/
}
}

#endif // SIGMA_FIVE_OPENGL_FRAME_BUFFER_HPP
