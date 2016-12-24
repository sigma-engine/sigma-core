#include <sigma/opengl/g_buffer.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {

    g_buffer::g_buffer()
        : size_(0, 0)
        , default_fbo_(0)
        , deffered_fbo_(0)
        , textures_{ 0, 0, 0, 0 }
        , depth_texture_(0)
    {
    }

    g_buffer::g_buffer(glm::ivec2 size)
        : g_buffer()
    {
        size_ = size;

        // Get the default draw frame buffer
        GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &default_fbo_));

        GL_CHECK(glGenFramebuffers(1, &deffered_fbo_));
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, deffered_fbo_));

        GL_CHECK(glGenTextures(GBUFFER_NUM_TEXTURES, textures_));
        GL_CHECK(glGenTextures(1, &depth_texture_));

        for (int i = 0; i < GBUFFER_NUM_TEXTURES; ++i) {
            GL_CHECK(glBindTexture(GL_TEXTURE_2D, textures_[i]));
            GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, size.x, size.y, 0, GL_RGB, GL_FLOAT, nullptr));
            GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures_[i], 0));
        }

        GL_CHECK(glBindTexture(GL_TEXTURE_2D, depth_texture_));
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture_, 0));

        GLenum drawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        GL_CHECK(glDrawBuffers(4, drawBuffers));

        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, default_fbo_));
    }

    g_buffer::~g_buffer()
    {
        GL_CHECK(glDeleteTextures(1, &depth_texture_));
        GL_CHECK(glDeleteTextures(GBUFFER_NUM_TEXTURES, textures_));
        GL_CHECK(glDeleteFramebuffers(1, &deffered_fbo_));
    }

    void g_buffer::bind_for_writting()
    {
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, deffered_fbo_));
    }

    void g_buffer::bind_for_reading()
    {
        GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, deffered_fbo_));
    }

    void g_buffer::set_read_buffer(g_buffer::GBUFFER_TEXTURE_TYPE type)
    {
        GL_CHECK(glReadBuffer(GL_COLOR_ATTACHMENT0 + type));
    }

    void g_buffer::bind_default()
    {
        GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, default_fbo_));
    }

    glm::ivec2 g_buffer::size() const
    {
        return size_;
    }

    void g_buffer::setSize(const glm::ivec2& size)
    {
        size_ = size;
    }
}
}
