#ifndef SIGMA_ENGINE_OPENGL_FRAME_BUFFER_HPP
#define SIGMA_ENGINE_OPENGL_FRAME_BUFFER_HPP

#include <sigma/opengl/gl_core_4_2.h>

#include <glm/vec2.hpp>

#include <memory>
#include <vector>

namespace sigma {
namespace opengl {
    class texture;

    class default_frame_buffer {
    public:
        enum class target {
            DRAW = GL_DRAW_FRAMEBUFFER,
            READ = GL_READ_FRAMEBUFFER,
        };

        default_frame_buffer(glm::ivec2 size);

        glm::ivec2 size();

        void bind();

        void bind(target tgt);

    protected:
        glm::ivec2 size_;
        GLuint object_;

    private:
        default_frame_buffer(const default_frame_buffer&) = delete;
        default_frame_buffer& operator=(const default_frame_buffer&) = delete;
    };

    // TODO hold on to bound textures???
    class frame_buffer : public default_frame_buffer {
    public:
        enum class attachment : GLenum {
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

        void attach(attachment att, texture& txt);

		void dettach(attachment att);

        void read_buffer(attachment att);

        void draw_buffers(std::vector<attachment> atts);

    private:
        frame_buffer(const frame_buffer&) = delete;
        frame_buffer& operator=(const frame_buffer&) = delete;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_FRAME_BUFFER_HPP
