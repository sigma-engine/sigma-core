#ifndef SIGMA_OPENGL_UNIFORM_BUFFER_HPP
#define SIGMA_OPENGL_UNIFORM_BUFFER_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/opengl/util.hpp>
#include <sigma/util/std140_conversion.hpp>

#include <glad/glad.h>

#include <cstring>

namespace sigma {
namespace opengl {

    template <class UnifomBufferType>
    class uniform_buffer {
    public:
        uniform_buffer()
        {
            GL_CHECK(glGenBuffers(1, &object_));
            GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, object_));
            GL_CHECK(glBufferData(GL_UNIFORM_BUFFER, std140_sizeof(UnifomBufferType), nullptr, GL_DYNAMIC_DRAW));
        }

        uniform_buffer(uniform_buffer&&) = default;

        uniform_buffer& operator=(uniform_buffer&&) = default;

        ~uniform_buffer()
        {
            glDeleteBuffers(1, &object_);
        }

        void set_data(const UnifomBufferType& data)
        {
            std::uint8_t buffer[std140_sizeof(UnifomBufferType)];
            std::ptrdiff_t offset = 0;
            std140::to_std140(data, buffer, offset);
            GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, object_));
            GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, 0, std140_sizeof(UnifomBufferType), buffer));
        }

        void set_binding_point(unsigned int index)
        {
            GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, index, object_));
        }

    private:
        uniform_buffer(const uniform_buffer&) = delete;
        uniform_buffer& operator=(const uniform_buffer&) = delete;

        GLuint object_ = 0;
    };
}
}

#endif // SIGMA_OPENGL_UNIFORM_BUFFER_HPP
