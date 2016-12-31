#ifndef SIGMA_ENGINE_OPENGL_geometry_BUFFER
#define SIGMA_ENGINE_OPENGL_geometry_BUFFER

#include <sigma/opengl/frame_buffer.hpp>
#include <sigma/opengl/texture.hpp>

#include <glm/vec4.hpp>

namespace sigma {
namespace opengl {
    class geometry_buffer : public frame_buffer {
    public:
        static constexpr const attachment WORLD_POSITION_ATTACHMENT = attachment::COLOR0;
        static constexpr const attachment DIFFUSE_COLOR_ATTACHMENT = attachment::COLOR1;
        static constexpr const attachment NORMAL_ATTACHMENT = attachment::COLOR2;
        static constexpr const attachment OUTPUT_IMAGE_ATTACHMENT = attachment::COLOR3;

        static constexpr const texture_unit WORLD_POSITION_TEXTURE_UINT = texture_unit::TEXTURE0;
        static constexpr const texture_unit DIFFUSE_COLOR_TEXTURE_UINT = texture_unit::TEXTURE1;
        static constexpr const texture_unit NORMAL_TEXTURE_UINT = texture_unit::TEXTURE2;
        static constexpr const texture_unit INPUT_IMAGE_TEXTURE_UINT = texture_unit::TEXTURE3;
        static constexpr const texture_unit DEPTH_STENCIL_TEXTURE_UINT = texture_unit::TEXTURE4;
        static constexpr const texture_unit NEXT_FREE_TEXTURE_UINT = texture_unit::TEXTURE5;

        static constexpr const char* WORLD_POSITION_OUTPUT_NAME = "position_output";
        static constexpr const char* DIFFUSE_COLOR_OUTPUT_NAME = "diffuse_output";
        static constexpr const char* NORMAL_OUTPUT_NAME = "normal_output";

        static constexpr const GLuint WORLD_POSITION_OUTPUT_LOCATION = 0;
        static constexpr const GLuint DIFFUSE_COLOR_OUTPUT_LOCATION = 1;
        static constexpr const GLuint NORMAL_OUTPUT_LOCATION = 2;
        static constexpr const GLuint INPUT_IMAGE_LOCATION = 3;
        static constexpr const GLuint DEPTH_STENCIL_LOCATION = 4;

        static constexpr const char* WORLD_POSITION_INPUT_NAME = "in_position";
        static constexpr const char* DIFFUSE_COLOR_INPUT_NAME = "in_diffuse";
        static constexpr const char* NORMAL_INPUT_NAME = "in_normal";
        static constexpr const char* IMAGE_INPUT_NAME = "in_image";
        static constexpr const char* DEPTH_STENCIL_INPUT_NAME = "in_depth_stencil";

        geometry_buffer(glm::ivec2 size);

        void bind_for_geometry_pass();

        void bind_for_stencil_pass();

        void bind_for_effect_pass();

        void clear_input_image(glm::vec4 color);

        void swap_input_image();

    private:
        geometry_buffer(const geometry_buffer&) = delete;
        geometry_buffer& operator=(const geometry_buffer&) = delete;

    private:
        texture position_texture_;
        texture diffuse_texture_;
        texture normal_texture_;
        texture depth_stencil_texture_;
        texture images_[2];
        int input_image_;
        int output_image_;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_geometry_BUFFER
