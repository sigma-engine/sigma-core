#include <sigma/graphics/opengl/render_buffer.hpp>

#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

namespace sigma {
namespace opengl {
    render_buffer::render_buffer(internal_format format, glm::ivec2 size)
        : size_(size)
    {
        GL_CHECK(glGenRenderbuffers(1, &object_));
        bind();
        GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GLenum(format), size_.x, size_.y));
    }

    render_buffer::~render_buffer()
    {
        glDeleteRenderbuffers(1, &object_);
    }

    void render_buffer::bind() const
    {
        GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, object_));
    }

    glm::ivec2 render_buffer::size() const
    {
        return size_;
    }

    GLuint render_buffer::get_object() const
    {
        return object_;
    }
}
}
