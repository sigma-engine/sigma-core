#ifndef SIGMA_ENGINE_OPENGL_FRAME_BUFFER_HPP
#define SIGMA_ENGINE_OPENGL_FRAME_BUFFER_HPP

#include <sigma/opengl/gl_core_4_2.h>

#include <glm/vec2.hpp>

#include <memory>
#include <vector>

namespace sigma {
namespace opengl {
    class texture;
    enum class frame_buffer_attachment : GLenum {
        COLOR_ATTACHMENT0 = GL_COLOR_ATTACHMENT0,
        COLOR_ATTACHMENT1 = GL_COLOR_ATTACHMENT1,
        COLOR_ATTACHMENT2 = GL_COLOR_ATTACHMENT2,
        COLOR_ATTACHMENT3 = GL_COLOR_ATTACHMENT3,
        COLOR_ATTACHMENT4 = GL_COLOR_ATTACHMENT4,
        COLOR_ATTACHMENT5 = GL_COLOR_ATTACHMENT5,
        COLOR_ATTACHMENT6 = GL_COLOR_ATTACHMENT6,
        COLOR_ATTACHMENT7 = GL_COLOR_ATTACHMENT7,
        COLOR_ATTACHMENT8 = GL_COLOR_ATTACHMENT8,
        COLOR_ATTACHMENT9 = GL_COLOR_ATTACHMENT9,
        DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
        STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT
    };
    class default_frame_buffer {
    public:
        default_frame_buffer(glm::ivec2 size);

        glm::ivec2 size();

        void bind_for_writting();

        void bind_for_reading();

        void bind();

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
        frame_buffer(glm::ivec2 size);

        ~frame_buffer();

        void attach(frame_buffer_attachment attachment, texture& txt);

        void draw_buffers(std::vector<frame_buffer_attachment> attachments);

    private:
        frame_buffer(const frame_buffer&) = delete;
        frame_buffer& operator=(const frame_buffer&) = delete;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_FRAME_BUFFER_HPP
