#include <sigma/opengl/shader_technique.hpp>

#include <sigma/opengl/geometry_buffer.hpp>
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

    void shader_technique::attach(resource::handle<graphics::shader> shdr)
    {
        assert(linked_ == GL_FALSE && "Can not add shaders to already linked programs");
        GL_CHECK(glAttachShader(object_, SHADER_CONST_PTR(shdr)->get_object()));
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
        GL_CHECK(standard_uniform_block_index_ = glGetUniformBlockIndex(object_, STANDARD_UNIFORM_BLOCK_NAME));
        GL_CHECK(glUniformBlockBinding(object_, standard_uniform_block_index_, STANDARD_UNIFORM_BLOCK_BINDING));

        GL_CHECK(model_matrix_location_ = glGetUniformLocation(object_, MODEL_MATRIX_NAME));
        GL_CHECK(model_view_matrix_location_ = glGetUniformLocation(object_, MODEL_VIEW_MATRIX_NAME));
        GL_CHECK(normal_matrix_location_ = glGetUniformLocation(object_, NORMAL_MATRIX_NAME));

        GL_CHECK(in_image_location_ = glGetUniformLocation(object_, geometry_buffer::IMAGE_INPUT_NAME));
    }

    GLint shader_technique::get_uniform_location(const char* name)
    {
        GL_CHECK(glUseProgram(object_));
        GL_CHECK(GLint loc = glGetUniformLocation(object_, name));
        return loc;
    }

    void shader_technique::set_texture(std::string name, resource::handle<graphics::texture> txt)
    {
        auto it = texture_map_.find(name);
        if (it == texture_map_.end()) {
            auto ind = get_uniform_location(name.c_str());
            // TODO maybe warn about the texture not being used?
            //assert(ind >= 0 && "Program does not have this texture!");
            if (ind >= 0) {
                texture_map_[name] = textures_.size();
                textures_.push_back(std::make_pair(ind, txt));
            }
            return;
        }
        textures_[texture_map_[name]].second = txt;
    }

	void shader_technique::set_instance_matrices(render_matrices* matrices) {
		GL_CHECK(glUniformMatrix4fv(model_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_matrix)));
		GL_CHECK(glUniformMatrix4fv(model_view_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_view_matrix)));
		GL_CHECK(glUniformMatrix3fv(normal_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->normal_matrix)));
	}

    void shader_technique::bind()
    {
        GL_CHECK(glUseProgram(object_));
        GL_CHECK(glUniform1i(in_image_location_, geometry_buffer::INPUT_IMAGE_LOCATION));
    }

    void shader_technique::bind(texture_unit first_texture_unit)
    {
        bind();
        auto size = textures_.size();
        for (uint32_t i = 0; i < size; ++i) {
            GL_CHECK(glUniform1i(textures_[i].first, i));
            GL_CHECK(glActiveTexture(GLenum(first_texture_unit) + i));
            TEXTURE_PTR(textures_[i].second)->bind();
        }
    }
}
}
