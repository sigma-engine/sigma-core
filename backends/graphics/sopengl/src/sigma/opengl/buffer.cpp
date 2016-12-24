#include <sigma/opengl/buffer.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    buffer::buffer(buffer_type type, buffer_usage usage)
        : type_(type)
        , usage_(usage)
        , gl_object_(0)
        , size_(0)
        , stride_(0)
    {
        GL_CHECK(glGenBuffers(1, &gl_object_));
    }

    buffer::~buffer()
    {
        glDeleteBuffers(1, &gl_object_); // TODO GL_CHECK
    }

    std::size_t buffer::size() const
    {
        return size_;
    }

    std::size_t buffer::stride() const
    {
        return stride_;
    }

    buffer::operator GLuint() const
    {
        return gl_object_;
    }

    void buffer::set_data(const void* data, std::size_t stride, std::size_t size)
    {
        // TODO preserve opengl buffer state??? or use direct state if is is
        // available.
        GL_CHECK(glBindBuffer(static_cast<GLenum>(type_), gl_object_));
        if (stride * size != stride_ * size_) {
            GL_CHECK(glBufferData(static_cast<GLenum>(type_), size * stride, data, static_cast<GLenum>(usage_)));
            size_ = size;
            stride_ = stride;
        } else {
            GL_CHECK(glBufferSubData(static_cast<GLenum>(type_), 0, size * stride, data));
        }
    }
}
}
