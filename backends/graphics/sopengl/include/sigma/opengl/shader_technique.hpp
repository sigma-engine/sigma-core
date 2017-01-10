#ifndef SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
#define SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP

#include <sigma/opengl/gl_core_4_2.h>
#include <sigma/opengl/util.hpp>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/texture.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace sigma {
namespace opengl {
    class shader;
    class texture;
    class cubemap;
    enum class texture_unit : GLenum;
    struct instance_matrices;
    struct standard_uniforms;

    template <class T>
    class shader_technique : public T {
    public:
        // static constexpr const char* STANDARD_UNIFORM_BLOCK_NAME = "standard_uniforms";
        // static constexpr const int STANDARD_UNIFORM_BLOCK_BINDING = 0;

        static constexpr const char* PROJECTION_MATRIX_NAME = "projection_matrix";
        static constexpr const char* VIEW_MATRIX_NAME = "view_matrix";
        static constexpr const char* Z_NEAR_NAME = "z_near";
        static constexpr const char* Z_FAR_NAME = "z_far";
        static constexpr const char* VIEW_PORT_SIZE_NAME = "view_port_size";
        static constexpr const char* TIME_NAME = "time";

        static constexpr const char* MODEL_MATRIX_NAME = "model_matrix";
        static constexpr const char* MODEL_VIEW_MATRIX_NAME = "model_view_matrix";
        static constexpr const char* NORMAL_MATRIX_NAME = "normal_matrix";

        shader_technique(const typename T::resource_data &data)
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

        // void attach(resource::handle<graphics::shader> shdr)
        // {
        //     assert(linked_ == GL_FALSE && "Can not add shaders to already linked programs");
        //     GL_CHECK(glAttachShader(object_, SHADER_CONST_PTR(shdr)->get_object()));
        //     shaders_.push_back(shdr);
        // }

        void link()
        {
            assert(linked_ == GL_FALSE && "Program already linked");

            for (const auto& shdr : shaders_)
                GL_CHECK(glAttachShader(object_, SHADER_CONST_PTR(shdr.second)->get_object()));

            GL_CHECK(glLinkProgram(object_));

            glGetProgramiv(object_, GL_LINK_STATUS, &linked_);
            if (linked_ == GL_FALSE) {
                std::cerr << "shader program: link faild" << std::endl;
                // TODO get the link message.
                std::abort();
            }

            GL_CHECK(glUseProgram(object_));
            // GL_CHECK(standard_uniform_block_index_ = glGetUniformBlockIndex(object_, STANDARD_UNIFORM_BLOCK_NAME));
            // GL_CHECK(glUniformBlockBinding(object_, standard_uniform_block_index_, STANDARD_UNIFORM_BLOCK_BINDING));

            GL_CHECK(projection_matrix_location_ = glGetUniformLocation(object_, PROJECTION_MATRIX_NAME));
            GL_CHECK(view_matrix_location_ = glGetUniformLocation(object_, VIEW_MATRIX_NAME));
            GL_CHECK(z_near_location_ = glGetUniformLocation(object_, Z_NEAR_NAME));
            GL_CHECK(z_far_location_ = glGetUniformLocation(object_, Z_FAR_NAME));
            GL_CHECK(view_port_size_location_ = glGetUniformLocation(object_, VIEW_PORT_SIZE_NAME));
            GL_CHECK(time_location_ = glGetUniformLocation(object_, TIME_NAME));

            GL_CHECK(model_matrix_location_ = glGetUniformLocation(object_, MODEL_MATRIX_NAME));
            GL_CHECK(model_view_matrix_location_ = glGetUniformLocation(object_, MODEL_VIEW_MATRIX_NAME));
            GL_CHECK(normal_matrix_location_ = glGetUniformLocation(object_, NORMAL_MATRIX_NAME));

            GL_CHECK(in_image_location_ = glGetUniformLocation(object_, geometry_buffer::IMAGE_INPUT_NAME));

            texture_locations_.resize(textures_.size());
            for (std::size_t i = 0; i < texture_locations_.size(); ++i) {
                GL_CHECK(texture_locations_[i] = glGetUniformLocation(object_, textures_[i].first.c_str()));
            }
        }

        //void set_texture(std::string name, resource::handle<graphics::texture> txt);

        void set_standard_uniforms(standard_uniforms* standard)
        {
            GL_CHECK(glUniformMatrix4fv(projection_matrix_location_, 1, GL_FALSE, glm::value_ptr(standard->projection_matrix)));
            GL_CHECK(glUniformMatrix4fv(view_matrix_location_, 1, GL_FALSE, glm::value_ptr(standard->view_matrix)));
            GL_CHECK(glUniform1f(z_near_location_, standard->z_near));
            GL_CHECK(glUniform1f(z_far_location_, standard->z_far));
            GL_CHECK(glUniform2fv(view_port_size_location_, 1, glm::value_ptr(standard->view_port_size)));
        }

        void set_instance_matrices(instance_matrices* matrices)
        {
            GL_CHECK(glUniformMatrix4fv(model_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_matrix)));
            GL_CHECK(glUniformMatrix4fv(model_view_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->model_view_matrix)));
            GL_CHECK(glUniformMatrix3fv(normal_matrix_location_, 1, GL_FALSE, glm::value_ptr(matrices->normal_matrix)));
        }

        void bind()
        {
            GL_CHECK(glUseProgram(object_));
            GL_CHECK(glUniform1i(in_image_location_, geometry_buffer::INPUT_IMAGE_LOCATION));
        }

        void bind(texture_unit first_texture_unit)
        {
            bind();
            auto start = GLenum(first_texture_unit) - GL_TEXTURE0;
            for (std::size_t i = 0; i < textures_.size(); ++i) {
                GL_CHECK(glActiveTexture(GLenum(first_texture_unit) + i));
                TEXTURE_PTR(textures_[i].second)->bind();
                GL_CHECK(glUniform1i(texture_locations_[i], i + start));
            }
        }

    protected:
        GLuint object_;
        GLint linked_ = GL_FALSE;

        // GLint standard_uniform_block_index_ = -1;

        GLint projection_matrix_location_ = -1;
        GLint view_matrix_location_ = -1;
        GLint z_near_location_ = -1;
        GLint z_far_location_ = -1;
        GLint view_port_size_location_ = -1;
        GLint time_location_ = -1;

        GLint model_matrix_location_ = -1;
        GLint model_view_matrix_location_ = -1;
        GLint normal_matrix_location_ = -1;

        std::vector<GLint> texture_locations_;
        // std::vector<GLint> cubemap_locations_;

        GLint in_image_location_ = -1;

    private:
        shader_technique(const shader_technique&) = delete;
        shader_technique& operator=(const shader_technique&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
