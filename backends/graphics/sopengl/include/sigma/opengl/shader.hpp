#ifndef SIGMA_ENGINE_SHADER_HPP
#define SIGMA_ENGINE_SHADER_HPP

#include <sigma/opengl/gl_core_4_0.h>

#include <string>

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

    class shader {
    public:
        shader(shader_type type, std::string source);

        ~shader();

        GLuint get_object() const;

    private:
        shader(const shader&) = delete;
        shader& operator=(const shader&) = delete;

        GLuint object_ = 0;
    };
}
}

#endif // SIGMA_ENGINE_SHADER_HPP
