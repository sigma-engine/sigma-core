#ifndef SIGMA_ENGINE_OPENGL_G_BUFFER
#define SIGMA_ENGINE_OPENGL_G_BUFFER

#include <glm/vec2.hpp>
#include <sigma/opengl/gl_core_4_0.h>

namespace sigma {
namespace opengl {
    class g_buffer {
    public:
        enum GBUFFER_TEXTURE_TYPE {
            GBUFFER_TEXTURE_TYPE_POSITION,
            GBUFFER_TEXTURE_TYPE_DIFFUSE,
            GBUFFER_TEXTURE_TYPE_NORMAL,
            GBUFFER_TEXTURE_TYPE_TEXCOORD,
            GBUFFER_NUM_TEXTURES
        };

        g_buffer(glm::ivec2 size);

        g_buffer(const g_buffer&) = delete;

        ~g_buffer();

        g_buffer& operator=(const g_buffer&) = delete;

        void bind_for_writting();

        void bind_for_reading();

        void set_read_buffer(GBUFFER_TEXTURE_TYPE type);

        void bind_default();

        glm::ivec2 size() const;

        void setSize(const glm::ivec2& size);

    private:
        glm::ivec2 size_;

        GLint default_fbo_;

        GLuint deffered_fbo_;
        GLuint textures_[GBUFFER_NUM_TEXTURES];
        GLuint depth_texture_;

        //GLint default_render_buffer_;
        //GLuint depth_buffer_;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_G_BUFFER
