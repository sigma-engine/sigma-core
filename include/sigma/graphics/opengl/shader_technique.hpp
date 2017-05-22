#ifndef SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
#define SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP

#include <sigma/graphics/opengl/cubemap.hpp>
#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/render_uniforms.hpp>
#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>
#include <sigma/graphics/standard_block.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace sigma {
namespace opengl {
    class shader_manager;

    template <class T>
    class shader_technique : public T {
    public:
        static constexpr const char* MODEL_MATRIX_NAME = "model_matrix";
        static constexpr const char* MODEL_VIEW_MATRIX_NAME = "model_view_matrix";
        static constexpr const char* NORMAL_MATRIX_NAME = "normal_matrix";

        shader_technique(const T& data)
            : T(data)
        {
            GL_CHECK(object_ = glCreateProgram());
        }

        shader_technique(shader_technique&&) = default;

        shader_technique& operator=(shader_technique&&) = default;

        ~shader_technique()
        {
            glDeleteProgram(object_);
        }

        void set_uniform(const std::string& name, float value)
        {
            for (auto& u : floats_) {
                if (u.first == name)
                    u.second = value;
            }
        }

        void set_uniform(const std::string& name, const glm::vec2& value)
        {
            for (auto& u : vec2s_) {
                if (u.first == name)
                    u.second = value;
            }
        }

        void set_uniform(const std::string& name, const glm::vec3& value)
        {
            for (auto& u : vec3s_) {
                if (u.first == name)
                    u.second = value;
            }
        }

        void set_uniform(const std::string& name, const glm::vec4& value)
        {
            for (auto& u : vec4s_) {
                if (u.first == name)
                    u.second = value;
            }
        }

        void link(texture_manager& texture_mgr, cubemap_manager& cubemap_mgr, shader_manager& shader_mgr)
        {
            assert(linked_ == GL_FALSE && "Program already linked");

            for (const auto& shdr : this->shaders) {
                auto shdr_hnd = shader_mgr.get(shdr.second);
                GL_CHECK(glAttachShader(object_, SHADER_PTR(shader_mgr, shdr_hnd)->get_object()));
            }

            GL_CHECK(glLinkProgram(object_));

            glGetProgramiv(object_, GL_LINK_STATUS, &linked_);
            if (linked_ == GL_FALSE) {
                std::cerr << "shader program: link faild\n";
                // TODO get the link message.
                std::abort();
            }

            GL_CHECK(glUseProgram(object_));

            GL_CHECK(model_matrix_location_ = glGetUniformLocation(object_, MODEL_MATRIX_NAME));
            GL_CHECK(model_view_matrix_location_ = glGetUniformLocation(object_, MODEL_VIEW_MATRIX_NAME));
            GL_CHECK(normal_matrix_location_ = glGetUniformLocation(object_, NORMAL_MATRIX_NAME));

            GL_CHECK(in_image_location_ = glGetUniformLocation(object_, geometry_buffer::IMAGE_INPUT_NAME));

            int i = 0;
            floats_.resize(T::floats.size());
            float_locations_.resize(T::floats.size());
            for (auto v : T::floats) {
                GL_CHECK(float_locations_[i] = glGetUniformLocation(object_, v.first.c_str()));
                floats_[i] = v;
                i++;
            }

            i = 0;
            vec2s_.resize(T::vec2s.size());
            vec2_locations_.resize(T::vec2s.size());
            for (auto v : T::vec2s) {
                GL_CHECK(vec2_locations_[i] = glGetUniformLocation(object_, v.first.c_str()));
                vec2s_[i] = v;
                i++;
            }

            i = 0;
            vec3s_.resize(T::vec3s.size());
            vec3_locations_.resize(T::vec3s.size());
            for (auto v : T::vec3s) {
                GL_CHECK(vec3_locations_[i] = glGetUniformLocation(object_, v.first.c_str()));
                vec3s_[i] = v;
                i++;
            }

            i = 0;
            vec4s_.resize(T::vec4s.size());
            vec4_locations_.resize(T::vec4s.size());
            for (auto v : T::vec4s) {
                GL_CHECK(vec4_locations_[i] = glGetUniformLocation(object_, v.first.c_str()));
                vec4s_[i] = v;
                i++;
            }

            i = 0;
            textures_.resize(T::textures.size());
            texture_locations_.resize(T::textures.size());
            for (auto v : T::textures) {
                GL_CHECK(texture_locations_[i] = glGetUniformLocation(object_, v.first.c_str()));
                textures_[i] = std::make_pair(v.first, texture_mgr.get(v.second));
                i++;
            }

            i = 0;
            cubemaps_.resize(T::cubemaps.size());
            cubemap_locations_.resize(T::cubemaps.size());
            for (auto v : T::cubemaps) {
                GL_CHECK(cubemap_locations_[i] = glGetUniformLocation(object_, v.first.c_str()));
                cubemaps_[i] = std::make_pair(v.first, cubemap_mgr.get(v.second));
                i++;
            }
        }

        void set_instance_matrices(instance_matrices* matrices) const
        {
            GL_CHECK(glUniformMatrix4fv(model_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_matrix)));
            GL_CHECK(glUniformMatrix4fv(model_view_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_view_matrix)));
            GL_CHECK(glUniformMatrix3fv(normal_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->normal_matrix)));
        }

        void bind(texture_manager& texture_mgr, cubemap_manager& cubemap_mgr) const
        {
            GL_CHECK(glUseProgram(object_));
            GL_CHECK(glUniform1i(in_image_location_, geometry_buffer::INPUT_IMAGE_LOCATION));

            for (std::size_t i = 0; i < this->floats_.size(); ++i) {
                // TODO this should not be needed because it should be checked
                // at compile time.
                if (float_locations_[i] >= 0) {
                    GL_CHECK(glUniform1f(float_locations_[i], this->floats_[i].second));
                }
            }

            for (std::size_t i = 0; i < this->vec2s_.size(); ++i) {
                // TODO this should not be needed because it should be checked
                // at compile time.
                if (vec2_locations_[i] >= 0) {
                    GL_CHECK(glUniform2fv(vec2_locations_[i], 1, glm::value_ptr(this->vec2s_[i].second)));
                }
            }

            for (std::size_t i = 0; i < this->vec3s_.size(); ++i) {
                // TODO this should not be needed because it should be checked
                // at compile time.
                if (vec3_locations_[i] >= 0) {
                    GL_CHECK(glUniform3fv(vec3_locations_[i], 1, glm::value_ptr(this->vec3s_[i].second)));
                }
            }

            for (std::size_t i = 0; i < this->vec4s_.size(); ++i) {
                // TODO this should not be needed because it should be checked
                // at compile time.
                if (vec4_locations_[i] >= 0) {
                    GL_CHECK(glUniform4fv(vec4_locations_[i], 1, glm::value_ptr(this->vec4s_[i].second)));
                }
            }
        }

        void bind(texture_manager& texture_mgr, cubemap_manager& cubemap_mgr, texture_unit first_texture_unit) const
        {
            bind(texture_mgr, cubemap_mgr);
            auto texture_unit = GLenum(first_texture_unit);
            auto unit_number = GLenum(first_texture_unit) - GL_TEXTURE0;
            for (std::size_t i = 0; i < this->textures_.size(); ++i, ++texture_unit, ++unit_number) {
                // TODO this should not be needed because it should be checked
                // at compile time.
                if (texture_locations_[i] >= 0) {
                    GL_CHECK(glActiveTexture(texture_unit));
                    TEXTURE_PTR(texture_mgr, this->textures_[i].second)->bind();
                    GL_CHECK(glUniform1i(texture_locations_[i], unit_number));
                }
            }

            for (std::size_t i = 0; i < this->cubemaps_.size(); ++i, ++texture_unit, ++unit_number) {
                // TODO this should not be needed because it should be checked
                // at compile time.
                if (cubemap_locations_[i] >= 0) {
                    GL_CHECK(glActiveTexture(texture_unit));
                    CUBEMAP_PTR(cubemap_mgr, this->cubemaps_[i].second)->bind();
                    GL_CHECK(glUniform1i(cubemap_locations_[i], unit_number));
                }
            }
        }

    protected:
        GLuint object_;
        GLint linked_ = GL_FALSE;

        GLint model_matrix_location_ = -1;
        GLint model_view_matrix_location_ = -1;
        GLint normal_matrix_location_ = -1;

        std::vector<std::pair<std::string, float>> floats_;
        std::vector<std::pair<std::string, glm::vec2>> vec2s_;
        std::vector<std::pair<std::string, glm::vec3>> vec3s_;
        std::vector<std::pair<std::string, glm::vec4>> vec4s_;
        std::vector<std::pair<std::string, resource::handle<graphics::texture>>> textures_;
        std::vector<std::pair<std::string, resource::handle<graphics::cubemap>>> cubemaps_;

        std::vector<GLint> float_locations_;
        std::vector<GLint> vec2_locations_;
        std::vector<GLint> vec3_locations_;
        std::vector<GLint> vec4_locations_;
        std::vector<GLint> texture_locations_;
        std::vector<GLint> cubemap_locations_;

        GLint in_image_location_ = -1;

    private:
        shader_technique(const shader_technique&) = delete;
        shader_technique& operator=(const shader_technique&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
