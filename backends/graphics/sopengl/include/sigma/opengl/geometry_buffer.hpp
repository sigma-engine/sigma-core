#ifndef SIGMA_ENGINE_OPENGL_geometry_BUFFER
#define SIGMA_ENGINE_OPENGL_geometry_BUFFER

#include <sigma/opengl/frame_buffer.hpp>
#include <sigma/opengl/texture.hpp>

namespace sigma {
namespace opengl {
    class geometry_buffer : public frame_buffer {
    public:
        geometry_buffer(glm::ivec2 size);

        void bind_textures();

    private:
        geometry_buffer(const geometry_buffer&) = delete;
        geometry_buffer& operator=(const geometry_buffer&) = delete;

    private:
        texture position_texture_;
        texture diffuse_texture_;
        texture normal_texture_;
        texture texcoord_texture_;
        texture depth_texture_;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_geometry_BUFFER
