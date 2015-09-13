#ifndef SIGMAFIVE_GRAPHICS_OPENGL_SHADER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_SHADER_HPP

#include <string>
#include <gl_core_4_0.hpp>

#define GLSL_110(x) "#version 110\n" #x
#define GLSL_120(x) "#version 120\n" #x
#define GLSL_130(x) "#version 130\n" #x
#define GLSL_140(x) "#version 140\n" #x
#define GLSL_150(x) "#version 150\n" #x

#define GLSL_330(x) "#version 330\n" #x
#define GLSL_400(x) "#version 400\n" #x
#define GLSL_410(x) "#version 410\n" #x
#define GLSL_420(x) "#version 420\n" #x
#define GLSL_430(x) "#version 430\n" #x
#define GLSL_440(x) "#version 440\n" #x

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            enum class shader_type {
                vertex = gl::VERTEX_SHADER,
                fragment = gl::FRAGMENT_SHADER
            };

            class shader {
            public:
                shader(shader_type type);

                ~shader();

                bool operator==(const shader &other) const;

                void set_source(std::string source);

                shader_type type() const;

                bool is_compiled() const;

                std::string compile();

                operator GLuint() const;

            private:
                shader_type type_;
                GLuint gl_object_;
                bool compiled_;
            };
        }
    }
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_SHADER_HPP
