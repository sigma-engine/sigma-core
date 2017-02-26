#ifndef SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
#define SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP

#include <sigma/graphics/opengl/cubemap.hpp>
#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/render_uniforms.hpp>
#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace sigma {
namespace opengl {

    template <class T>
    class shader_technique : public T {
    public:
        // static constexpr const char* STANDARD_UNIFORM_BLOCK_NAME = "standard_uniforms";
        // static constexpr const int STANDARD_UNIFORM_BLOCK_BINDING = 0;

        static constexpr const char* PROJECTION_MATRIX_NAME = "projection_matrix";
        static constexpr const char* VIEW_MATRIX_NAME = "view_matrix";
        static constexpr const char* INVERSE_PROJECTION_MATRIX_NAME = "inverse_projection_matrix";
        static constexpr const char* INVERSE_VIEW_MATRIX_NAME = "inverse_view_matrix";
        static constexpr const char* Z_NEAR_NAME = "z_near";
        static constexpr const char* Z_FAR_NAME = "z_far";
        static constexpr const char* VIEW_PORT_SIZE_NAME = "view_port_size";
        static constexpr const char* TIME_NAME = "time";

        static constexpr const char* MODEL_MATRIX_NAME = "model_matrix";
        static constexpr const char* MODEL_VIEW_MATRIX_NAME = "model_view_matrix";
        static constexpr const char* NORMAL_MATRIX_NAME = "normal_matrix";

        shader_technique(const typename T::resource_data& data)
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

        void link()
        {
            assert(linked_ == GL_FALSE && "Program already linked");

            for (const auto& shdr : this->shaders_)
                GL_CHECK(glAttachShader(object_, SHADER_CONST_PTR(shdr.second)->get_object()));

            GL_CHECK(glLinkProgram(object_));

            glGetProgramiv(object_, GL_LINK_STATUS, &linked_);
            if (linked_ == GL_FALSE) {
                std::cerr << "shader program: link faild\n";
                // TODO get the link message.
                std::abort();
            }

            GL_CHECK(glUseProgram(object_));
            // GL_CHECK(standard_uniform_block_index_ = glGetUniformBlockIndex(object_, STANDARD_UNIFORM_BLOCK_NAME));
            // GL_CHECK(glUniformBlockBinding(object_, standard_uniform_block_index_, STANDARD_UNIFORM_BLOCK_BINDING));

            GL_CHECK(projection_matrix_location_ = glGetUniformLocation(object_, PROJECTION_MATRIX_NAME));
            GL_CHECK(view_matrix_location_ = glGetUniformLocation(object_, VIEW_MATRIX_NAME));
            GL_CHECK(inverse_projection_matrix_location_ = glGetUniformLocation(object_, INVERSE_PROJECTION_MATRIX_NAME));
            GL_CHECK(inverse_view_matrix_location_ = glGetUniformLocation(object_, INVERSE_VIEW_MATRIX_NAME));
            GL_CHECK(z_near_location_ = glGetUniformLocation(object_, Z_NEAR_NAME));
            GL_CHECK(z_far_location_ = glGetUniformLocation(object_, Z_FAR_NAME));
            GL_CHECK(view_port_size_location_ = glGetUniformLocation(object_, VIEW_PORT_SIZE_NAME));
            GL_CHECK(time_location_ = glGetUniformLocation(object_, TIME_NAME));

            GL_CHECK(model_matrix_location_ = glGetUniformLocation(object_, MODEL_MATRIX_NAME));
            GL_CHECK(model_view_matrix_location_ = glGetUniformLocation(object_, MODEL_VIEW_MATRIX_NAME));
            GL_CHECK(normal_matrix_location_ = glGetUniformLocation(object_, NORMAL_MATRIX_NAME));

            GL_CHECK(in_image_location_ = glGetUniformLocation(object_, geometry_buffer::IMAGE_INPUT_NAME));

            float_locations_.resize(this->floats_.size());
            for (std::size_t i = 0; i < float_locations_.size(); ++i) {
                GL_CHECK(float_locations_[i] = glGetUniformLocation(object_, this->floats_[i].first.c_str()));
            }

            vec2_locations_.resize(this->vec2s_.size());
            for (std::size_t i = 0; i < vec2_locations_.size(); ++i) {
                GL_CHECK(vec2_locations_[i] = glGetUniformLocation(object_, this->vec2s_[i].first.c_str()));
            }

            vec3_locations_.resize(this->vec3s_.size());
            for (std::size_t i = 0; i < vec3_locations_.size(); ++i) {
                GL_CHECK(vec3_locations_[i] = glGetUniformLocation(object_, this->vec3s_[i].first.c_str()));
            }

            vec4_locations_.resize(this->vec4s_.size());
            for (std::size_t i = 0; i < vec4_locations_.size(); ++i) {
                GL_CHECK(vec4_locations_[i] = glGetUniformLocation(object_, this->vec4s_[i].first.c_str()));
            }

            texture_locations_.resize(this->textures_.size());
            for (std::size_t i = 0; i < texture_locations_.size(); ++i) {
                GL_CHECK(texture_locations_[i] = glGetUniformLocation(object_, this->textures_[i].first.c_str()));
            }

            cubemap_locations_.resize(this->cubemaps_.size());
            for (std::size_t i = 0; i < cubemap_locations_.size(); ++i) {
                GL_CHECK(cubemap_locations_[i] = glGetUniformLocation(object_, this->cubemaps_[i].first.c_str()));
            }
        }

        void set_standard_uniforms(standard_uniforms* standard) const
        {
            GL_CHECK(glUniformMatrix4fv(projection_matrix_location_, 1, GL_FALSE, glm::value_ptr(standard->projection_matrix)));
            GL_CHECK(glUniformMatrix4fv(view_matrix_location_, 1, GL_FALSE, glm::value_ptr(standard->view_matrix)));
            GL_CHECK(glUniformMatrix4fv(inverse_projection_matrix_location_, 1, GL_FALSE, glm::value_ptr(standard->inverse_projection_matrix)));
            GL_CHECK(glUniformMatrix4fv(inverse_view_matrix_location_, 1, GL_FALSE, glm::value_ptr(standard->inverse_view_matrix)));
            GL_CHECK(glUniform1f(z_near_location_, standard->z_near));
            GL_CHECK(glUniform1f(z_far_location_, standard->z_far));
            GL_CHECK(glUniform2fv(view_port_size_location_, 1, glm::value_ptr(standard->view_port_size)));
        }

        void set_instance_matrices(instance_matrices* matrices) const
        {
            GL_CHECK(glUniformMatrix4fv(model_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_matrix)));
            GL_CHECK(glUniformMatrix4fv(model_view_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_view_matrix)));
            GL_CHECK(glUniformMatrix3fv(normal_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->normal_matrix)));
        }

        void bind() const
        {
            GL_CHECK(glUseProgram(object_));
            GL_CHECK(glUniform1i(in_image_location_, geometry_buffer::INPUT_IMAGE_LOCATION));

            for (std::size_t i = 0; i < this->floats_.size(); ++i)
                GL_CHECK(glUniform1f(float_locations_[i], this->floats_[i].second));

            for (std::size_t i = 0; i < this->vec2s_.size(); ++i)
                GL_CHECK(glUniform2fv(vec2_locations_[i], 1, glm::value_ptr(this->vec2s_[i].second)));

            for (std::size_t i = 0; i < this->vec3s_.size(); ++i)
                GL_CHECK(glUniform3fv(vec3_locations_[i], 1, glm::value_ptr(this->vec3s_[i].second)));

            for (std::size_t i = 0; i < this->vec4s_.size(); ++i)
                GL_CHECK(glUniform4fv(vec4_locations_[i], 1, glm::value_ptr(this->vec4s_[i].second)));
        }

        void bind(texture_unit first_texture_unit) const
        {
            bind();
            auto texture_unit = GLenum(first_texture_unit);
            auto unit_number = GLenum(first_texture_unit) - GL_TEXTURE0;
            for (std::size_t i = 0; i < this->textures_.size(); ++i, ++texture_unit, ++unit_number) {
                GL_CHECK(glActiveTexture(texture_unit));
                TEXTURE_CONST_PTR(this->textures_[i].second)->bind();
                GL_CHECK(glUniform1i(texture_locations_[i], unit_number));
            }

            for (std::size_t i = 0; i < this->cubemaps_.size(); ++i, ++texture_unit, ++unit_number) {
                GL_CHECK(glActiveTexture(texture_unit));
                CUBEMAP_CONST_PTR(this->cubemaps_[i].second)->bind();
                GL_CHECK(glUniform1i(cubemap_locations_[i], unit_number));
            }
        }

    protected:
        GLuint object_;
        GLint linked_ = GL_FALSE;

        // GLint standard_uniform_block_index_ = -1;

        GLint projection_matrix_location_ = -1;
        GLint view_matrix_location_ = -1;
        GLint inverse_projection_matrix_location_ = -1;
        GLint inverse_view_matrix_location_ = -1;
        GLint z_near_location_ = -1;
        GLint z_far_location_ = -1;
        GLint view_port_size_location_ = -1;
        GLint time_location_ = -1;

        GLint model_matrix_location_ = -1;
        GLint model_view_matrix_location_ = -1;
        GLint normal_matrix_location_ = -1;

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

    template <class InternalType, class Cache>
    class shader_technique_manager : public Cache {
    public:
        shader_technique_manager(boost::filesystem::path cache_directory, opengl::shader_manager& shaders, opengl::texture_manager& textures, opengl::cubemap_manager& cubemaps)
            : Cache(cache_directory)
            , shaders_(shaders)
            , textures_(textures)
            , cubemaps_(cubemaps)
        {
        }

        virtual std::unique_ptr<typename Cache::resource_type> create(typename Cache::resource_data data) override
        {
            auto tech = std::make_unique<InternalType>(data);

            for (auto type : graphics::all_shader_types()) {
                if (tech->has_shader(type))
                    tech->shader(type).set_manager(&shaders_);
            }
            tech->link();

            for (unsigned int i = 0; i < tech->texture_count(); ++i)
                tech->texture(i).set_manager(&textures_);

            for (unsigned int i = 0; i < tech->cubemap_count(); ++i)
                tech->cubemap(i).set_manager(&cubemaps_);

            return std::move(tech);
        }

    private:
        opengl::shader_manager& shaders_;
        opengl::texture_manager& textures_;
        opengl::cubemap_manager& cubemaps_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
