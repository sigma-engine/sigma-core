#ifndef SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
#define SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP

#include <sigma/graphics/opengl/cubemap.hpp>
#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/render_uniforms.hpp>
#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>
#include <sigma/graphics/standard_block.hpp>
#include <sigma/graphics/technique.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace sigma {
namespace opengl {
    class shader_manager;

    class technique {
    public:
        static constexpr const char* MODEL_MATRIX_NAME = "model_matrix";
        static constexpr const char* MODEL_VIEW_MATRIX_NAME = "model_view_matrix";
        static constexpr const char* NORMAL_MATRIX_NAME = "normal_matrix";

        technique(shader_manager& shader_mgr, const sigma::graphics::technique_identifier& technique_id, const sigma::graphics::technique_uniform_data& data)
        {
            GL_CHECK(object_ = glCreateProgram());

            auto vertex_shader = shader_mgr.get(technique_id.vertex);
            GL_CHECK(glAttachShader(object_, SHADER_PTR(shader_mgr, vertex_shader)->get_object()));

            if (technique_id.tessellation_control.size() > 0) {
                auto tessellation_control_shader = shader_mgr.get(technique_id.tessellation_control);
                GL_CHECK(glAttachShader(object_, SHADER_PTR(shader_mgr, tessellation_control_shader)->get_object()));
            }

            if (technique_id.tessellation_evaluation.size() > 0) {
                auto tessellation_evaluation_shader = shader_mgr.get(technique_id.tessellation_evaluation);
                GL_CHECK(glAttachShader(object_, SHADER_PTR(shader_mgr, tessellation_evaluation_shader)->get_object()));
            }

            if (technique_id.geometry.size() > 0) {
                auto geometry_shader = shader_mgr.get(technique_id.geometry);
                GL_CHECK(glAttachShader(object_, SHADER_PTR(shader_mgr, geometry_shader)->get_object()));
            }

            auto fragment_shader = shader_mgr.get(technique_id.fragment);
            GL_CHECK(glAttachShader(object_, SHADER_PTR(shader_mgr, fragment_shader)->get_object()));

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

        technique(technique&&) = default;

        technique& operator=(technique&&) = default;

        ~technique()
        {
            glDeleteProgram(object_);
        }

        void set_uniform(const std::string& name, float value)
        {
            // TODO this should not be needed because it should be checked
            // at compile time.
            auto it = uniform_locations_.find(name);
            if (it != uniform_locations_.end()) {
                GL_CHECK(glUniform1f(it->second, value));
            }
        }

        void set_uniform(const std::string& name, const glm::vec2& value)
        {
            // TODO this should not be needed because it should be checked
            // at compile time.
            auto it = uniform_locations_.find(name);
            if (it != uniform_locations_.end()) {
                GL_CHECK(glUniform2fv(it->second, 1, glm::value_ptr(value)));
            }
        }

        void set_uniform(const std::string& name, const glm::vec3& value)
        {
            // TODO this should not be needed because it should be checked
            // at compile time.
            auto it = uniform_locations_.find(name);
            if (it != uniform_locations_.end()) {
                GL_CHECK(glUniform3fv(it->second, 1, glm::value_ptr(value)));
            }
        }

        void set_uniform(const std::string& name, const glm::vec4& value)
        {
            // TODO this should not be needed because it should be checked
            // at compile time.
            auto it = uniform_locations_.find(name);
            if (it != uniform_locations_.end()) {
                GL_CHECK(glUniform4fv(it->second, 1, glm::value_ptr(value)));
            }
        }

        void set_instance_matrices(instance_matrices* matrices) const
        {
            GL_CHECK(glUniformMatrix4fv(model_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_matrix)));
            GL_CHECK(glUniformMatrix4fv(model_view_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_view_matrix)));
            GL_CHECK(glUniformMatrix3fv(normal_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->normal_matrix)));
        }

        void bind(const sigma::graphics::technique_uniform_data& data)
        {
            GL_CHECK(glUseProgram(object_));
            GL_CHECK(glUniform1i(in_image_location_, geometry_buffer::INPUT_IMAGE_LOCATION));

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

        void bind(texture_manager& texture_mgr, cubemap_manager& cubemap_mgr, const sigma::graphics::technique_uniform_data& data, texture_unit first_texture_unit)
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
                    auto hndl = texture_mgr.get(uniform.second);
                    TEXTURE_PTR(texture_mgr, hndl)->bind();
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
                    auto hndl = cubemap_mgr.get(uniform.second);
                    CUBEMAP_PTR(cubemap_mgr, hndl)->bind();
                    GL_CHECK(glUniform1i(it->second, unit_number));
                    unit_number++;
                    texture_unit++;
                }
            }
        }

    protected:
        GLuint object_;

        GLint model_matrix_location_ = -1;
        GLint model_view_matrix_location_ = -1;
        GLint normal_matrix_location_ = -1;
        GLint in_image_location_ = -1;

        std::unordered_map<std::string, GLint> uniform_locations_;

    private:
        technique(const technique&) = delete;
        technique& operator=(const technique&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
