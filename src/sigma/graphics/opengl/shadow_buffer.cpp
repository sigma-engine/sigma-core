#include <sigma/graphics/opengl/shadow_buffer.hpp>

#include <sigma/graphics/opengl/util.hpp>

namespace sigma {
namespace opengl {
    shadow_buffer::shadow_buffer(glm::ivec2 size)
        : frame_buffer(size)
        , shadow_map_(internal_format::RG32F, size, graphics::texture_filter::LINEAR, graphics::texture_filter::LINEAR)
    {
        // TODO move this into texture
        shadow_map_.bind();
        //GLfloat border_color[] = { 1.0, 1.0, 0.0, 0.0 };
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        //GL_CHECK(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color));

        attach(frame_buffer::attachment::COLOR0, shadow_map_);

        GL_CHECK(glGenRenderbuffers(1, &depth_render_buffer_));
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer_));
        GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, size_.x, size_.y));
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer_));
    }

    shadow_buffer::~shadow_buffer()
    {
        glDeleteRenderbuffers(1, &depth_render_buffer_);
    }

    void shadow_buffer::bind_for_shadow_write()
    {
        draw_buffers(frame_buffer::attachment::COLOR0);
        GL_CHECK(glViewport(0, 0, size_.x, size_.y));
    }

    void shadow_buffer::bind_for_shadow_read(texture_unit unit)
    {
        GL_CHECK(glViewport(0, 0, size_.x, size_.y));

        GL_CHECK(glActiveTexture(GLenum(unit)));
        shadow_map_.bind();
    }
}
}
