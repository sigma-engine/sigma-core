#ifndef SIGMA_GRAPHICS_OPENGL_FRAME_BUFFER_HPP
#define SIGMA_GRAPHICS_OPENGL_FRAME_BUFFER_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

#include <glm/vec2.hpp>

#include <array>

namespace sigma {
namespace opengl {
    class texture;
    class render_buffer;

    // TODO hold on to bound textures???
    class frame_buffer {
    public:
        enum class target {
            DRAW = GL_DRAW_FRAMEBUFFER,
            READ = GL_READ_FRAMEBUFFER,
        };

        enum class attachment : GLenum {
            FRONT_LEFT = GL_FRONT_LEFT,
            FRONT_RIGHT = GL_FRONT_RIGHT,
            BACK_LEFT = GL_BACK_LEFT,
            BACK_RIGHT = GL_BACK_RIGHT,
            FRONT = GL_FRONT,
            BACK = GL_BACK,
            LEFT = GL_LEFT,
            RIGHT = GL_RIGHT,
            COLOR0 = GL_COLOR_ATTACHMENT0,
            COLOR1 = GL_COLOR_ATTACHMENT1,
            COLOR2 = GL_COLOR_ATTACHMENT2,
            COLOR3 = GL_COLOR_ATTACHMENT3,
            COLOR4 = GL_COLOR_ATTACHMENT4,
            COLOR5 = GL_COLOR_ATTACHMENT5,
            COLOR6 = GL_COLOR_ATTACHMENT6,
            COLOR7 = GL_COLOR_ATTACHMENT7,
            COLOR8 = GL_COLOR_ATTACHMENT8,
            COLOR9 = GL_COLOR_ATTACHMENT9,
            DEPTH = GL_DEPTH_ATTACHMENT,
            STENCIL = GL_STENCIL_ATTACHMENT,
            DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT,
            NONE = GL_NONE
        };

        frame_buffer(glm::ivec2 size);

        ~frame_buffer();

        const glm::ivec2& size() const noexcept;

        void bind();

        void bind(target tgt);

        void attach(attachment att, const texture& txt);

        void attach(attachment att, const render_buffer& buf);

        void dettach(attachment att, const texture& txt);

        void dettach(attachment att, const render_buffer& txt);

        void read_buffer(attachment att);

        template <class... Args>
        void draw_buffers(Args... atts)
        {
            std::array<attachment, sizeof...(Args)> real = { { atts... } };
            GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, object_));
            GL_CHECK(glDrawBuffers(sizeof...(Args), (GLenum*)real.data()));
        }

        static frame_buffer get_current();

    protected:
    private:
        frame_buffer(GLuint fbo, const glm::ivec2& size, bool should_delete);

        frame_buffer(const frame_buffer&) = delete;
        frame_buffer& operator=(const frame_buffer&) = delete;

        glm::ivec2 size_;
        bool should_delete_;
        GLuint object_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_FRAME_BUFFER_HPP
