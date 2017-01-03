#ifndef SIGMA_ENGINE_SHADER_HPP
#define SIGMA_ENGINE_SHADER_HPP

#include <sigma/graphics/shader.hpp>

#include <sigma/opengl/gl_core_4_2.h>

#define SHADER_CONST_PTR(x) static_cast<const sigma::opengl::shader*>(x.get())
#define SHADER_PTR(x) static_cast<sigma::opengl::shader*>(x.get())

namespace sigma {
namespace opengl {
    enum class shader_type : GLenum {
        //COMPUTE_SHADER = GL_COMPUTE_SHADER,
        VERTEX_SHADER = GL_VERTEX_SHADER,
        TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
        TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
        GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER
    };

    class shader : public graphics::shader {
    public:
        shader(shader_type type, std::string source);

        shader(graphics::shader_data data);

        shader(shader&&) = default;

        shader& operator=(shader&&) = default;

        virtual ~shader();

        GLuint get_object() const;

    private:
        shader(const shader&) = delete;
        shader& operator=(const shader&) = delete;

        GLuint object_ = 0;
    };

    class shader_manager : public graphics::shader_manager {
    public:
        using graphics::shader_manager::shader_manager;

        virtual std::unique_ptr<graphics::shader> load(graphics::shader_data data, boost::archive::binary_iarchive &ia) override;
    };
}
}

#endif // SIGMA_ENGINE_SHADER_HPP
