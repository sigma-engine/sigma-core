#include <sigma/graphics/opengl/technique.hpp>

#include <sigma/graphics/opengl/cubemap_manager.hpp>
#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/render_uniforms.hpp>
#include <sigma/graphics/opengl/shader_manager.hpp>
#include <sigma/graphics/standard_block.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace sigma {
namespace opengl {
    technique::technique(shader_manager& shader_mgr, const sigma::graphics::technique& data)
    {
        object_ = glCreateProgram();

        glAttachShader(object_, shader_mgr.acquire(data.vertex));

        if (data.tessellation_control.is_valid())
            glAttachShader(object_, shader_mgr.acquire(data.tessellation_control));

        if (data.tessellation_evaluation.is_valid())
            glAttachShader(object_, shader_mgr.acquire(data.tessellation_evaluation));

        if (data.geometry.is_valid())
            glAttachShader(object_, shader_mgr.acquire(data.geometry));

        glAttachShader(object_, shader_mgr.acquire(data.fragment));

        glLinkProgram(object_);

        GLint linkded = GL_FALSE;
        glGetProgramiv(object_, GL_LINK_STATUS, &linkded);
        if (linkded == GL_FALSE) {
            std::cerr << "shader program: link faild\n";
            // TODO get the link message.
            std::abort();
        }

        glUseProgram(object_);

        model_matrix_location_ = glGetUniformLocation(object_, MODEL_MATRIX_NAME);
        model_view_matrix_location_ = glGetUniformLocation(object_, MODEL_VIEW_MATRIX_NAME);
        normal_matrix_location_ = glGetUniformLocation(object_, NORMAL_MATRIX_NAME);
        in_image_location_ = glGetUniformLocation(object_, geometry_buffer::IMAGE_INPUT_NAME);
        in_diffuse_location_ = glGetUniformLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_INPUT_NAME);
        in_normal_location_ = glGetUniformLocation(object_, geometry_buffer::NORMAL_METALNESS_INPUT_NAME);
        in_depth_stencil_location_ = glGetUniformLocation(object_, geometry_buffer::DEPTH_STENCIL_INPUT_NAME);
        in_shadow_map0_location_ = glGetUniformLocation(object_, geometry_buffer::SHADOW_MAP0_INPUT_NAME);
        in_shadow_map1_location_ = glGetUniformLocation(object_, geometry_buffer::SHADOW_MAP1_INPUT_NAME);
        in_shadow_map2_location_ = glGetUniformLocation(object_, geometry_buffer::SHADOW_MAP2_INPUT_NAME);

        glUniform1i(in_diffuse_location_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION);
        glUniform1i(in_normal_location_, geometry_buffer::NORMAL_METALNESS_LOCATION);
        glUniform1i(in_depth_stencil_location_, geometry_buffer::DEPTH_STENCIL_LOCATION);
        glUniform1i(in_shadow_map0_location_, geometry_buffer::SHADOW_MAP0_LOCATION);
        glUniform1i(in_shadow_map1_location_, geometry_buffer::SHADOW_MAP1_LOCATION);
        glUniform1i(in_shadow_map2_location_, geometry_buffer::SHADOW_MAP2_LOCATION);
        glBindFragDataLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_NAME);
        glBindFragDataLocation(object_, geometry_buffer::NORMAL_METALNESS_LOCATION, geometry_buffer::NORMAL_METALNESS_OUTPUT_NAME);
    }

    technique::~technique()
    {
        glDeleteProgram(object_);
    }

    void technique::gather_uniform_locations(const sigma::graphics::technique_uniform_data& data)
    {
        for (auto v : data.floats) {
            uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str());
        }

        for (auto v : data.vec2s) {
            uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str());
        }

        for (auto v : data.vec3s) {
            uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str());
        }

        for (auto v : data.vec4s) {
            uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str());
        }

        for (auto v : data.textures) {
            uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str());
        }

        for (auto v : data.cubemaps) {
            uniform_locations_[v.first] = glGetUniformLocation(object_, v.first.c_str());
        }
    }

    void technique::set_uniform(const std::string& name, float value)
    {
        // TODO this should not be needed because it should be checked
        // at compile time.
        auto it = uniform_locations_.find(name);
        if (it != uniform_locations_.end()) {
            glUniform1f(it->second, value);
        }
    }

    void technique::set_uniform(const std::string& name, const glm::vec2& value)
    {
        // TODO this should not be needed because it should be checked
        // at compile time.
        auto it = uniform_locations_.find(name);
        if (it != uniform_locations_.end()) {
            glUniform2fv(it->second, 1, glm::value_ptr(value));
        }
    }

    void technique::set_uniform(const std::string& name, const glm::vec3& value)
    {
        // TODO this should not be needed because it should be checked
        // at compile time.
        auto it = uniform_locations_.find(name);
        if (it != uniform_locations_.end()) {
            glUniform3fv(it->second, 1, glm::value_ptr(value));
        }
    }

    void technique::set_uniform(const std::string& name, const glm::vec4& value)
    {
        // TODO this should not be needed because it should be checked
        // at compile time.
        auto it = uniform_locations_.find(name);
        if (it != uniform_locations_.end()) {
            glUniform4fv(it->second, 1, glm::value_ptr(value));
        }
    }

    void technique::set_instance_matrices(instance_matrices* matrices) const
    {
        glUniformMatrix4fv(model_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_matrix));
        glUniformMatrix4fv(model_view_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_view_matrix));
        glUniformMatrix3fv(normal_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->normal_matrix));
    }

    void technique::bind()
    {
        glUseProgram(object_);
        glUniform1i(in_image_location_, geometry_buffer::INPUT_IMAGE_LOCATION);
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

    void technique::bind(texture_manager& texture_mgr, cubemap_manager& cubemap_mgr, const sigma::graphics::technique_uniform_data& data, GLenum first_texture_unit)
    {
        bind(data);

        auto texture_unit = GLenum(first_texture_unit);
        auto unit_number = GLenum(first_texture_unit) - GL_TEXTURE0;

        for (const auto& uniform : data.textures) {
            // TODO this should not be needed because it should be checked
            // at compile time.
            auto it = uniform_locations_.find(uniform.first);
            if (it != uniform_locations_.end()) {
                glActiveTexture(texture_unit);
                glBindTexture(GL_TEXTURE_2D, texture_mgr.acquire(uniform.second));
                glUniform1i(it->second, unit_number);
                unit_number++;
                texture_unit++;
            }
        }

        for (const auto& uniform : data.cubemaps) {
            // TODO this should not be needed because it should be checked
            // at compile time.
            auto it = uniform_locations_.find(uniform.first);
            if (it != uniform_locations_.end()) {
                glActiveTexture(texture_unit);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_mgr.acquire(uniform.second));
                glUniform1i(it->second, unit_number);
                unit_number++;
                texture_unit++;
            }
        }
    }
}
}
