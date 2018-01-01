#include <sigma/graphics/opengl/technique.hpp>

#include <sigma/graphics/opengl/cubemap.hpp>
#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/render_uniforms.hpp>
#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/standard_block.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace sigma {
namespace opengl {
    technique::technique(shader_manager& shader_mgr, const sigma::graphics::technique& data)
    {
        GL_CHECK(object_ = glCreateProgram());

        GL_CHECK(glAttachShader(object_, SHADER_PTR(shader_mgr, data.vertex)->get_object()));

        if (data.tessellation_control.is_valid()) {
            GL_CHECK(glAttachShader(object_, SHADER_PTR(shader_mgr, data.tessellation_control)->get_object()));
        }

        if (data.tessellation_evaluation.is_valid()) {
            GL_CHECK(glAttachShader(object_, SHADER_PTR(shader_mgr, data.tessellation_evaluation)->get_object()));
        }

        if (data.geometry.is_valid()) {
            GL_CHECK(glAttachShader(object_, SHADER_PTR(shader_mgr, data.geometry)->get_object()));
        }

        GL_CHECK(glAttachShader(object_, SHADER_PTR(shader_mgr, data.fragment)->get_object()));

        GL_CHECK(glLinkProgram(object_));

        GLint linkded = GL_FALSE;
        glGetProgramiv(object_, GL_LINK_STATUS, &linkded);
        if (linkded == GL_FALSE) {
            std::cerr << "shader program: link faild\n";
            // TODO get the link message.
            std::abort();
        }

        GL_CHECK(glUseProgram(object_));

        GL_CHECK(model_matrix_location_ = glGetUniformLocation(object_, MODEL_MATRIX_NAME));
        GL_CHECK(model_view_matrix_location_ = glGetUniformLocation(object_, MODEL_VIEW_MATRIX_NAME));
        GL_CHECK(normal_matrix_location_ = glGetUniformLocation(object_, NORMAL_MATRIX_NAME));
        GL_CHECK(in_image_location_ = glGetUniformLocation(object_, geometry_buffer::IMAGE_INPUT_NAME));
        GL_CHECK(in_diffuse_location_ = glGetUniformLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_INPUT_NAME));
        GL_CHECK(in_normal_location_ = glGetUniformLocation(object_, geometry_buffer::NORMAL_METALNESS_INPUT_NAME));
        GL_CHECK(in_depth_stencil_location_ = glGetUniformLocation(object_, geometry_buffer::DEPTH_STENCIL_INPUT_NAME));
        GL_CHECK(in_shadow_map0_location_ = glGetUniformLocation(object_, geometry_buffer::SHADOW_MAP0_INPUT_NAME));
        GL_CHECK(in_shadow_map1_location_ = glGetUniformLocation(object_, geometry_buffer::SHADOW_MAP1_INPUT_NAME));
        GL_CHECK(in_shadow_map2_location_ = glGetUniformLocation(object_, geometry_buffer::SHADOW_MAP2_INPUT_NAME));

        GL_CHECK(glUniform1i(in_diffuse_location_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION));
        GL_CHECK(glUniform1i(in_normal_location_, geometry_buffer::NORMAL_METALNESS_LOCATION));
        GL_CHECK(glUniform1i(in_depth_stencil_location_, geometry_buffer::DEPTH_STENCIL_LOCATION));
        GL_CHECK(glUniform1i(in_shadow_map0_location_, geometry_buffer::SHADOW_MAP0_LOCATION));
        GL_CHECK(glUniform1i(in_shadow_map1_location_, geometry_buffer::SHADOW_MAP1_LOCATION));
        GL_CHECK(glUniform1i(in_shadow_map2_location_, geometry_buffer::SHADOW_MAP2_LOCATION));
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::NORMAL_METALNESS_LOCATION, geometry_buffer::NORMAL_METALNESS_OUTPUT_NAME));
    }

    technique::~technique()
    {
        glDeleteProgram(object_);
    }

    void technique::gather_uniform_locations(const sigma::graphics::technique_uniform_data& data)
    {
        for (auto v : data.floats) {
            GL_CHECK(uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str()));
        }

        for (auto v : data.vec2s) {
            GL_CHECK(uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str()));
        }

        for (auto v : data.vec3s) {
            GL_CHECK(uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str()));
        }

        for (auto v : data.vec4s) {
            GL_CHECK(uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str()));
        }

        for (auto v : data.textures) {
            GL_CHECK(uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str()));
        }

        for (auto v : data.cubemaps) {
            GL_CHECK(uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str()));
        }
    }

    void technique::set_uniform(const std::string& name, float value)
    {
        // TODO this should not be needed because it should be checked
        // at compile time.
        auto it = uniform_locations_.find(name);
        if (it != uniform_locations_.end()) {
            GL_CHECK(glUniform1f(it->second, value));
        }
    }

    void technique::set_uniform(const std::string& name, const glm::vec2& value)
    {
        // TODO this should not be needed because it should be checked
        // at compile time.
        auto it = uniform_locations_.find(name);
        if (it != uniform_locations_.end()) {
            GL_CHECK(glUniform2fv(it->second, 1, glm::value_ptr(value)));
        }
    }

    void technique::set_uniform(const std::string& name, const glm::vec3& value)
    {
        // TODO this should not be needed because it should be checked
        // at compile time.
        auto it = uniform_locations_.find(name);
        if (it != uniform_locations_.end()) {
            GL_CHECK(glUniform3fv(it->second, 1, glm::value_ptr(value)));
        }
    }

    void technique::set_uniform(const std::string& name, const glm::vec4& value)
    {
        // TODO this should not be needed because it should be checked
        // at compile time.
        auto it = uniform_locations_.find(name);
        if (it != uniform_locations_.end()) {
            GL_CHECK(glUniform4fv(it->second, 1, glm::value_ptr(value)));
        }
    }

    void technique::set_instance_matrices(instance_matrices* matrices) const
    {
        GL_CHECK(glUniformMatrix4fv(model_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_matrix)));
        GL_CHECK(glUniformMatrix4fv(model_view_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_view_matrix)));
        GL_CHECK(glUniformMatrix3fv(normal_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->normal_matrix)));
    }

    void technique::bind()
    {
        GL_CHECK(glUseProgram(object_));
        GL_CHECK(glUniform1i(in_image_location_, geometry_buffer::INPUT_IMAGE_LOCATION));
    }

    void technique::bind(const sigma::graphics::technique_uniform_data& data)
    {
        bind();

        for (const auto& uniform : data.floats) {
            this->set_uniform(uniform.first, uniform.second);
        }

        for (const auto& uniform : data.vec2s) {
            this->set_uniform(uniform.first, uniform.second);
        }

        for (const auto& uniform : data.vec3s) {
            this->set_uniform(uniform.first, uniform.second);
        }

        for (const auto& uniform : data.vec4s) {
            this->set_uniform(uniform.first, uniform.second);
        }
    }

    void technique::bind(texture_manager& texture_mgr, cubemap_manager& cubemap_mgr, const sigma::graphics::technique_uniform_data& data, texture_unit first_texture_unit)
    {
        bind(data);

        auto texture_unit = GLenum(first_texture_unit);
        auto unit_number = GLenum(first_texture_unit) - GL_TEXTURE0;

        for (const auto& uniform : data.textures) {
            // TODO this should not be needed because it should be checked
            // at compile time.
            auto it = uniform_locations_.find(uniform.first);
            if (it != uniform_locations_.end()) {
                GL_CHECK(glActiveTexture(texture_unit));
                // TODO this is terrible
                TEXTURE_PTR(texture_mgr, uniform.second)->bind();
                GL_CHECK(glUniform1i(it->second, unit_number));
                unit_number++;
                texture_unit++;
            }
        }

        for (const auto& uniform : data.cubemaps) {
            // TODO this should not be needed because it should be checked
            // at compile time.
            auto it = uniform_locations_.find(uniform.first);
            if (it != uniform_locations_.end()) {
                GL_CHECK(glActiveTexture(texture_unit));
                // TODO this is terrible
                CUBEMAP_PTR(cubemap_mgr, uniform.second)->bind();
                GL_CHECK(glUniform1i(it->second, unit_number));
                unit_number++;
                texture_unit++;
            }
        }
    }
}
}
