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
        GL_CHECK(world_position_input_location_ = glGetUniformLocation(object_, geometry_buffer::WORLD_POSITION_INPUT_NAME));
        GL_CHECK(diffuse_color_input_location_ = glGetUniformLocation(object_, geometry_buffer::DIFFUSE_COLOR_INPUT_NAME));
        GL_CHECK(normal_input_location_ = glGetUniformLocation(object_, geometry_buffer::NORMAL_INPUT_NAME));
    }

    void post_process_effect::apply(render_matrices* matrices)
    {
        this->bind(matrices, geometry_buffer::FINAL_IMAGE_TEXTURE_UINT);
        GL_CHECK(glUniform1i(world_position_input_location_, geometry_buffer::WORLD_POSITION_OUTPUT_LOCATION));
        GL_CHECK(glUniform1i(diffuse_color_input_location_, geometry_buffer::DIFFUSE_COLOR_OUTPUT_LOCATION));
        GL_CHECK(glUniform1i(normal_input_location_, geometry_buffer::NORMAL_OUTPUT_LOCATION));
        mesh_->render();
    }
}
}
