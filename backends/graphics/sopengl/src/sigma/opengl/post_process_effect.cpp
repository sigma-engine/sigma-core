#include <sigma/opengl/post_process_effect.hpp>

#include <sigma/opengl/geometry_buffer.hpp>
#include <sigma/opengl/static_mesh.hpp>
#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    post_process_effect::post_process_effect(std::shared_ptr<static_mesh> mesh)
        : mesh_(mesh)
    {
    }

    void post_process_effect::link()
    {
        shader_technique::link();
        GL_CHECK(in_diffuse_location_ = glGetUniformLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_INPUT_NAME));
        GL_CHECK(in_normal_location_ = glGetUniformLocation(object_, geometry_buffer::NORMAL_METALNESS_INPUT_NAME));
        GL_CHECK(in_image_location_ = glGetUniformLocation(object_, geometry_buffer::IMAGE_INPUT_NAME));
        GL_CHECK(in_depth_stencil_location_ = glGetUniformLocation(object_, geometry_buffer::DEPTH_STENCIL_INPUT_NAME));
    }

    void post_process_effect::apply(render_matrices* matrices)
    {
        this->bind(matrices, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
        GL_CHECK(glUniform1i(in_diffuse_location_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION));
        GL_CHECK(glUniform1i(in_normal_location_, geometry_buffer::NORMAL_METALNESS_LOCATION));
        GL_CHECK(glUniform1i(in_image_location_, geometry_buffer::INPUT_IMAGE_LOCATION));
        GL_CHECK(glUniform1i(in_depth_stencil_location_, geometry_buffer::DEPTH_STENCIL_LOCATION));
        mesh_->render();
    }
}
}
