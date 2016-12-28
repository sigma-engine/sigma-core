#ifndef SIGMA_ENGINE_OPENGL_geometry_BUFFER
#define SIGMA_ENGINE_OPENGL_geometry_BUFFER

#include <sigma/opengl/frame_buffer.hpp>
#include <sigma/opengl/texture.hpp>

namespace sigma {
namespace opengl {
    class geometry_buffer : public frame_buffer {
    public:
        static constexpr const char* WORLD_POSITION_OUTPUT_NAME = "position_output";
        static constexpr const char* DIFFUSE_COLOR_OUTPUT_NAME = "diffuse_output";
        static constexpr const char* NORMAL_OUTPUT_NAME = "normal_output";

        static constexpr const GLuint WORLD_POSITION_OUTPUT_LOCATION = 0;
        static constexpr const GLuint DIFFUSE_COLOR_OUTPUT_LOCATION = 1;
        static constexpr const GLuint NORMAL_OUTPUT_LOCATION = 2;

        static constexpr const char* WORLD_POSITION_INPUT_NAME = "in_position";
        static constexpr const char* DIFFUSE_COLOR_INPUT_NAME = "in_diffuse";
        static constexpr const char* NORMAL_INPUT_NAME = "in_normal";

        geometry_buffer(glm::ivec2 size);

        void bind_textures();

    private:
        geometry_buffer(const geometry_buffer&) = delete;
        geometry_buffer& operator=(const geometry_buffer&) = delete;

    private:
        texture position_texture_;
        texture diffuse_texture_;
        texture normal_texture_;
        texture depth_stencil_texture_;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_geometry_BUFFER
