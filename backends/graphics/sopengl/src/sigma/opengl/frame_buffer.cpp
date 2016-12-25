#include <sigma/opengl/frame_buffer.hpp>

#include <sigma/opengl/texture.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    default_frame_buffer::default_frame_buffer(glm::ivec2 size)
        : size_(size)
    {
        GLint d;
        GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &d));
        object_ = d;
    }

    glm::ivec2 default_frame_buffer::size()
    {
        return size_;
    }

    void default_frame_buffer::bind_for_writting()
    {
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, object_));
    }

    void default_frame_buffer::bind_for_reading()
    {
        GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, object_));
    }

    void default_frame_buffer::bind()
    {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, object_));
    }

    frame_buffer::frame_buffer(glm::ivec2 size)
        : default_frame_buffer(size)
    {
        GL_CHECK(glGenFramebuffers(1, &object_));
    }

    frame_buffer::~frame_buffer()
    {
        glDeleteFramebuffers(1, &object_);
    }

    void frame_buffer::attach(frame_buffer_attachment attachment, texture& txt)
    {
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, object_));
        txt.bind();
        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GLenum(attachment), GL_TEXTURE_2D, txt.get_object(), 0));
    }

    void frame_buffer::draw_buffers(std::vector<frame_buffer_attachment> attachments)
    {
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, object_));
        // TODO
        GL_CHECK(glDrawBuffers(attachments.size(), (GLenum*)attachments.data()));
    }
}
}
