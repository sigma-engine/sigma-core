#ifndef SIGMA_ENGINE_MATERIAL_HPP
#define SIGMA_ENGINE_MATERIAL_HPP

#include <sigma/opengl/gl_core_4_2.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace sigma {
namespace opengl {
    class shader;
    class texture;
    struct render_matrices;
    class material {
    public:
        static constexpr const char* PROJECTION_MATRIX_NAME = "projectionMatrix";
        static constexpr const char* VIEW_MATRIX_NAME = "viewMatrix";
        static constexpr const char* MODEL_MATRIX_NAME = "modelMatrix";
        static constexpr const char* MODEL_VIEW_MATRIX_NAME = "modelViewMatrix";
        static constexpr const char* NORMAL_MATRIX_NAME = "normalMatrix";
        static constexpr const char* TIME_NAME = "time";

        material();

        ~material();

        void attach(std::shared_ptr<shader> shdr);

        void link();

        void set_texture(std::string name, std::shared_ptr<texture> txt);

        void bind(render_matrices* matrices);

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;

        GLuint object_ = 0;
        GLint projection_matrix_location_ = -1;
        GLint view_matrix_location_ = -1;
        GLint model_matrix_location_ = -1;
        GLint model_view_matrix_location_ = -1;
        GLint normal_matrix_location_ = -1;
        GLint time_location_ = -1;

        GLint linked_ = GL_FALSE;
        std::vector<std::shared_ptr<shader>> shaders_;
        std::unordered_map<std::string, std::size_t> texture_map_;
        std::vector<std::pair<GLint, std::shared_ptr<texture>>> textures_;
    };
}
}

#endif // SIGMA_ENGINE_MATERIAL_HPP
