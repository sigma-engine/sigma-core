#include <sigma/graphics/opengl/frame_buffer.hpp>

#include <sigma/graphics/opengl/render_buffer.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

namespace sigma {
namespace opengl {

    frame_buffer::frame_buffer(glm::ivec2 size)
        : size_(size)
        , should_delete_(true)
    {
        GL_CHECK(glGenFramebuffers(1, &object_));
    }

    frame_buffer::frame_buffer(GLuint fbo, const glm::ivec2& size, bool should_delete)
        : size_(size)
        , should_delete_(should_delete)
        , object_(fbo)
    {
    }

    frame_buffer::~frame_buffer()
    {
        if (should_delete_)
            glDeleteFramebuffers(1, &object_);
    }

    const glm::ivec2& frame_buffer::size() const noexcept
    {
        return size_;
    }

    void frame_buffer::bind()
    {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, object_));
    }

    void frame_buffer::bind(target tgt)
    {
        GL_CHECK(glBindFramebuffer(GLenum(tgt), object_));
    }

    void frame_buffer::attach(attachment att, const texture& txt)
    {
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, object_));
        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GLenum(att), GL_TEXTURE_2D, txt.get_object(), 0));
    }

    void frame_buffer::attach(attachment att, const render_buffer& buf)
    {
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, object_));
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GLenum(att), GL_RENDERBUFFER, buf.get_object()));
    }

    void frame_buffer::dettach(attachment att, const texture& txt)
    {
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, object_));
        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GLenum(att), GL_TEXTURE_2D, 0, 0));
    }

    void frame_buffer::dettach(attachment att, const render_buffer& txt)
    {
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, object_));
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GLenum(att), GL_RENDERBUFFER, 0));
    }

    void frame_buffer::read_buffer(attachment att)
    {
        GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, object_));
        GL_CHECK(glReadBuffer(GLenum(att)));
    }

    frame_buffer frame_buffer::get_current()
    {
        GLint fbo;
        GLint dims[4] = { 0 };
        GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo));
        GL_CHECK(glGetIntegerv(GL_VIEWPORT, dims));
        return { (GLuint)fbo, { dims[2], dims[3] }, false };
    }
}
}
