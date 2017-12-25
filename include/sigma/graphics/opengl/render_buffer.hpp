#ifndef SIGMA_OPENGL_RENDER_BUFFER_HPP
#define SIGMA_OPENGL_RENDER_BUFFER_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/opengl/texture.hpp>

#include <glad/glad.h>

#include <glm/vec2.hpp>

namespace sigma {
namespace opengl {
    class render_buffer {
    public:
        render_buffer(internal_format format, glm::ivec2 size);

        ~render_buffer();

        void bind() const;

        glm::ivec2 size() const;

        GLuint get_object() const;

    private:
        render_buffer(const render_buffer&) = delete;
        render_buffer& operator=(const render_buffer&) = delete;

        glm::ivec2 size_;
        GLuint object_;
    };
}
}

#endif // SIGMA_OPENGL_RENDER_BUFFER_HPP
