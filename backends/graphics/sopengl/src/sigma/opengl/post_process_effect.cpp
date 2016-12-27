#include <sigma/opengl/post_process_effect.hpp>

#include <sigma/opengl/material.hpp> // TODO remove this
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
        GL_CHECK(world_position_input_location_ = glGetUniformLocation(object_, WORLD_POSITION_INPUT_NAME));
        GL_CHECK(diffuse_color_input_location_ = glGetUniformLocation(object_, DIFFUSE_COLOR_INPUT_NAME));
        GL_CHECK(normal_input_location_ = glGetUniformLocation(object_, NORMAL_INPUT_NAME));
        GL_CHECK(texture_coordinate_input_location_ = glGetUniformLocation(object_, TEXTURE_COORDINATE_INPUT_NAME));
    }

    void post_process_effect::apply(render_matrices* matrices)
    {
        this->bind(matrices, 4);
        GL_CHECK(glUniform1i(world_position_input_location_, material::WORLD_POSITION_OUTPUT_LOCATION));
        GL_CHECK(glUniform1i(diffuse_color_input_location_, material::DIFFUSE_COLOR_OUTPUT_LOCATION));
        GL_CHECK(glUniform1i(normal_input_location_, material::NORMAL_OUTPUT_LOCATION));
        GL_CHECK(glUniform1i(texture_coordinate_input_location_, material::TEXTURE_COORDINATE_OUTPUT_LOCATION));
        mesh_->render();
    }

    /*void post_process_effect::apply(render_matrices* matrices, int texture_unit)
    {
        this->bind(matrices, 4);
        auto ind = glGetUniformLocation(object_, "input_image");
        GL_CHECK(glUniform1i(ind, texture_unit));
        mesh_->render();
    }*/
}
}
