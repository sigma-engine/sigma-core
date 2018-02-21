#ifndef SIGMA_GRAPHICS_OPENGL_GEOMETRY_BUFFER_HPP
#define SIGMA_GRAPHICS_OPENGL_GEOMETRY_BUFFER_HPP

#include <glad/glad.h>

namespace sigma {
namespace opengl {
    class geometry_buffer {
    public:
        static constexpr const GLenum DIFFUSE_ROUGHNESS_ATTACHMENT = GL_COLOR_ATTACHMENT0;
        static constexpr const GLenum NORMAL_METALNESS_ATTACHMENT = GL_COLOR_ATTACHMENT1;
        static constexpr const GLenum IMAGE_ATTACHMENTS[] = { GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

        static constexpr const GLenum DIFFUSE_ROUGHNESS_TEXTURE_UINT = GL_TEXTURE0;
        static constexpr const GLenum NORMAL_METALNESS_TEXTURE_UINT = GL_TEXTURE1;
        static constexpr const GLenum DEPTH_STENCIL_TEXTURE_UINT = GL_TEXTURE2;
        static constexpr const GLenum INPUT_IMAGE_TEXTURE_UINT = GL_TEXTURE3;
        static constexpr const GLenum SHADOW_MAP_TEXTURE_UINT = GL_TEXTURE4;
        static constexpr const GLenum NEXT_FREE_TEXTURE_UINT = GL_TEXTURE5;

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
        static constexpr const char* SHADOW_MAP_NAME = "in_shadow_map";
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_GEOMETRY_BUFFER_HPP
