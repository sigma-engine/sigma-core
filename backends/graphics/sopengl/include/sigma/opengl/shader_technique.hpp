#ifndef SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
#define SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP

#include <sigma/opengl/gl_core_4_2.h>
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
    enum class texture_unit : GLenum;
    struct render_matrices;
    struct standard_uniforms;
    class shader_technique {
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

        shader_technique();

        shader_technique(shader_technique&&) = default;

        shader_technique& operator=(shader_technique&&) = default;

        ~shader_technique();

        void attach(resource::handle<graphics::shader> shdr);

        void link();

        GLint get_uniform_location(const char* name);

        void set_texture(std::string name, resource::handle<graphics::texture> txt);

        void set_instance_matrices(standard_uniforms* standard, render_matrices* matrices);

        void bind();

        void bind(texture_unit first_texture_unit);

        GLuint object_;

    protected:
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

        GLint linked_ = GL_FALSE;
        std::vector<resource::handle<graphics::shader>> shaders_;
        std::unordered_map<std::string, std::size_t> texture_map_;
        std::vector<std::pair<GLint, resource::handle<graphics::texture>>> textures_;

    private:
        GLint in_image_location_ = -1;

    private:
        shader_technique(const shader_technique&) = delete;
        shader_technique& operator=(const shader_technique&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
