#include <sigma/opengl/material.hpp>

#include <sigma/opengl/render_uniforms.hpp>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/texture.hpp>
#include <sigma/opengl/util.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <iostream>

namespace sigma {
namespace opengl {
    material::material()
    {
        GL_CHECK(object_ = glCreateProgram());
    }

    material::~material()
    {
        glDeleteProgram(object_);
    }

    void material::attach(std::shared_ptr<shader> shdr)
    {
        assert(linked_ == GL_FALSE && "Can not add shaders to already linked programs");
        GL_CHECK(glAttachShader(object_, shdr->get_object()));
        shaders_.push_back(shdr);
    }

    void material::set_texture(std::string name, std::shared_ptr<texture> txt)
    {
        auto it = texture_map_.find(name);
        if (it == texture_map_.end()) {
            auto ind = glGetUniformLocation(object_, name.c_str());
            assert(ind >= 0 && "Program does not have this texture!");
            if (ind >= 0) {
                texture_map_[name] = textures_.size();
                textures_.push_back(std::make_pair(textures_.size(), txt));
            }
            return;
        }
        textures_[texture_map_[name]].second = txt;
    }

    void material::link()
    {
        assert(linked_ == GL_FALSE && "Program already linked");
        GL_CHECK(glLinkProgram(object_));

        glGetProgramiv(object_, GL_LINK_STATUS, &linked_);
        if (linked_ == GL_FALSE) {
            std::cerr << "shader program: link faild" << std::endl;
            // TODO get the link message.
            std::abort();
        }

        GL_CHECK(projection_matrix_location_ = glGetUniformLocation(object_, PROJECTION_MATRIX_NAME));
        GL_CHECK(view_matrix_location_ = glGetUniformLocation(object_, VIEW_MATRIX_NAME));
        GL_CHECK(model_matrix_location_ = glGetUniformLocation(object_, MODEL_MATRIX_NAME));
        GL_CHECK(model_view_matrix_location_ = glGetUniformLocation(object_, MODEL_VIEW_MATRIX_NAME));
        GL_CHECK(normal_matrix_location_ = glGetUniformLocation(object_, NORMAL_MATRIX_NAME));
        GL_CHECK(time_location_ = glGetUniformLocation(object_, TIME_NAME));

        GL_CHECK(glBindFragDataLocation(object_, WORLD_POSITION_OUTPUT_LOCATION, WORLD_POSITION_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, DIFFUSE_COLOR_OUTPUT_LOCATION, DIFFUSE_COLOR_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, NORMAL_OUTPUT_LOCATION, NORMAL_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, TEXTURE_COORDINATE_OUTPUT_LOCATION, TEXTURE_COORDINATE_OUTPUT_NAME));
    }

    void material::bind(render_matrices* matrices)
    {
        GL_CHECK(glUseProgram(object_));
        auto size = textures_.size();
        for (uint32_t i = 0; i < size; ++i) {
            GL_CHECK(glUniform1i(textures_[i].first, i));
            GL_CHECK(glActiveTexture(GL_TEXTURE0 + i));
            textures_[i].second->bind();
        }

        GL_CHECK(glUniformMatrix4fv(projection_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->projection_matrix)));
        GL_CHECK(glUniformMatrix4fv(view_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->view_matrix)));
        GL_CHECK(glUniformMatrix4fv(model_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_matrix)));
        GL_CHECK(glUniformMatrix4fv(model_view_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_view_matrix)));
        GL_CHECK(glUniformMatrix3fv(normal_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->normal_matrix)));
    }
}
}
