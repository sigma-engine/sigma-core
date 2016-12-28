#include <sigma/opengl/shader_technique.hpp>

#include <sigma/opengl/render_uniforms.hpp>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/texture.hpp>
#include <sigma/opengl/util.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <iostream>

namespace sigma {
namespace opengl {
    shader_technique::shader_technique()
    {
        GL_CHECK(object_ = glCreateProgram());
    }

    shader_technique::~shader_technique()
    {
        glDeleteProgram(object_);
    }

    void shader_technique::attach(std::shared_ptr<shader> shdr)
    {
        assert(linked_ == GL_FALSE && "Can not add shaders to already linked programs");
        GL_CHECK(glAttachShader(object_, shdr->get_object()));
        shaders_.push_back(shdr);
    }

    void shader_technique::link()
    {
        assert(linked_ == GL_FALSE && "Program already linked");
        GL_CHECK(glLinkProgram(object_));

        glGetProgramiv(object_, GL_LINK_STATUS, &linked_);
        if (linked_ == GL_FALSE) {
            std::cerr << "shader program: link faild" << std::endl;
            // TODO get the link message.
            std::abort();
        }

        GL_CHECK(glUseProgram(object_));
        GL_CHECK(projection_matrix_location_ = glGetUniformLocation(object_, PROJECTION_MATRIX_NAME));
        GL_CHECK(view_matrix_location_ = glGetUniformLocation(object_, VIEW_MATRIX_NAME));
        GL_CHECK(model_matrix_location_ = glGetUniformLocation(object_, MODEL_MATRIX_NAME));
        GL_CHECK(model_view_matrix_location_ = glGetUniformLocation(object_, MODEL_VIEW_MATRIX_NAME));
        GL_CHECK(normal_matrix_location_ = glGetUniformLocation(object_, NORMAL_MATRIX_NAME));
        GL_CHECK(view_port_size_location_ = glGetUniformLocation(object_, VIEW_PORT_SIZE_NAME));
        GL_CHECK(time_location_ = glGetUniformLocation(object_, TIME_NAME));
    }

    GLint shader_technique::get_uniform_location(const char* name)
    {
        GL_CHECK(glUseProgram(object_));
        GL_CHECK(GLint loc = glGetUniformLocation(object_, name));
        return loc;
    }

    void shader_technique::set_texture(std::string name, std::shared_ptr<texture> txt)
    {
        auto it = texture_map_.find(name);
        if (it == texture_map_.end()) {
            auto ind = get_uniform_location(name.c_str());
            // TODO maybe warn about the texture not being used?
            //assert(ind >= 0 && "Program does not have this texture!");
            if (ind >= 0) {
                texture_map_[name] = textures_.size();
                textures_.push_back(std::make_pair(textures_.size(), txt));
            }
            return;
        }
        textures_[texture_map_[name]].second = txt;
    }

	void shader_technique::bind()
	{
		GL_CHECK(glUseProgram(object_));
	}

    void shader_technique::bind(render_matrices* matrices, texture_unit first_texture_unit)
    {
        GL_CHECK(glUseProgram(object_));
        auto size = textures_.size();
        for (uint32_t i = 0; i < size; ++i) {
            GL_CHECK(glUniform1i(textures_[i].first, i));
            GL_CHECK(glActiveTexture(GLenum(first_texture_unit) + i));
            textures_[i].second->bind();
        }

        GL_CHECK(glUniformMatrix4fv(projection_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->projection_matrix)));
        GL_CHECK(glUniformMatrix4fv(view_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->view_matrix)));
        GL_CHECK(glUniformMatrix4fv(model_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_matrix)));
        GL_CHECK(glUniformMatrix4fv(model_view_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_view_matrix)));
        GL_CHECK(glUniformMatrix3fv(normal_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->normal_matrix)));
        GL_CHECK(glUniform2fv(view_port_size_location_, 1, glm::value_ptr(matrices->size)));
    }
}
}
