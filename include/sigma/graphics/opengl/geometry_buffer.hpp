#ifndef SIGMA_GRAPHICS_OPENGL_GEOMETRY_BUFFER_HPP
#define SIGMA_GRAPHICS_OPENGL_GEOMETRY_BUFFER_HPP

#include <sigma/graphics/opengl/frame_buffer.hpp>
#include <sigma/graphics/opengl/texture.hpp>

#include <glm/vec4.hpp>

namespace sigma {
namespace opengl {
    class geometry_buffer : public frame_buffer {
    public:
        static constexpr const attachment DIFFUSE_ROUGHNESS_ATTACHMENT = attachment::COLOR0;
        static constexpr const attachment NORMAL_METALNESS_ATTACHMENT = attachment::COLOR1;
        static constexpr const attachment IMAGE_ATTACHMENTS[] = { attachment::COLOR2, attachment::COLOR3 };

        static constexpr const texture_unit DIFFUSE_ROUGHNESS_TEXTURE_UINT = texture_unit::TEXTURE0;
        static constexpr const texture_unit NORMAL_METALNESS_TEXTURE_UINT = texture_unit::TEXTURE1;
        static constexpr const texture_unit DEPTH_STENCIL_TEXTURE_UINT = texture_unit::TEXTURE2;
        static constexpr const texture_unit INPUT_IMAGE_TEXTURE_UINT = texture_unit::TEXTURE3;
        static constexpr const texture_unit SHADOW_MAP_TEXTURE_UINT = texture_unit::TEXTURE4;
        static constexpr const texture_unit NEXT_FREE_TEXTURE_UINT = texture_unit::TEXTURE5;

        static constexpr const char* DIFFUSE_ROUGHNESS_OUTPUT_NAME = "out_diffuse_roughness";
        static constexpr const char* NORMAL_METALNESS_OUTPUT_NAME = "out_normal_metalness";

        static constexpr const GLuint DIFFUSE_ROUGHNESS_OUTPUT_LOCATION = 0;
        static constexpr const GLuint NORMAL_METALNESS_LOCATION = 1;
        static constexpr const GLuint DEPTH_STENCIL_LOCATION = 2;
        static constexpr const GLuint INPUT_IMAGE_LOCATION = 3;
        static constexpr const GLuint SHADOW_MAP_LOCATION = 4;

        static constexpr const char* DIFFUSE_ROUGHNESS_INPUT_NAME = "in_diffuse_roughness";
        static constexpr const char* NORMAL_METALNESS_INPUT_NAME = "in_normal_metalness";
        static constexpr const char* DEPTH_STENCIL_INPUT_NAME = "in_depth_stencil";
        static constexpr const char* IMAGE_INPUT_NAME = "in_image";
        static constexpr const char* SHADOW_MAP_INPUT_NAME = "in_shadow_map";

        geometry_buffer(glm::ivec2 size);

        void bind_for_geometry_write();

        void bind_for_geometry_read();

        void swap_input_image();

    private:
        geometry_buffer(const geometry_buffer&) = delete;
        geometry_buffer& operator=(const geometry_buffer&) = delete;

    private:
        texture diffuse_texture_;
        texture normal_texture_;
        texture depth_stencil_texture_;
        texture images_[2];
        int input_image_;
        int output_image_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_GEOMETRY_BUFFER_HPP
