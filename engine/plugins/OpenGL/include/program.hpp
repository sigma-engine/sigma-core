#ifndef SIGMAFIVE_GRAPHICS_OPENGL_PROGRAM_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_PROGRAM_HPP

#include <shader.hpp>
#include <texture_2d.hpp>
#include <gl_core_4_0.hpp>

#include <sigmafive/math/vec2.hpp>
#include <sigmafive/math/vec3.hpp>
#include <sigmafive/math/vec4.hpp>
#include <sigmafive/math/mat2.hpp>
#include <sigmafive/math/mat3.hpp>
#include <sigmafive/math/mat4.hpp>
#include <sigmafive/math/quaternion.hpp>

#include <string>
#include <vector>
#include <functional>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            class program {
            public:
                program();

                ~program();

                bool is_linked() const;

                void attach(shader &shader);

                void detach(shader &shader);

                std::string link();

                GLint get_uniform_location(std::string name) const;

                template<typename T>
                void set_uniform(const std::string &name, T &&value) {
                    set_uniform(get_uniform_location(name), std::forward<T>(value));
                }

                void set_uniform(GLint location, bool value);

                void set_uniform(GLint location, int value);

                void set_uniform(GLint location, float value);

                void set_uniform(GLint location, vec2_t<bool> value);

                void set_uniform(GLint location, vec2_t<int> value);

                void set_uniform(GLint location, vec2_t<float> value);

                void set_uniform(GLint location, vec3_t<bool> value);

                void set_uniform(GLint location, vec3_t<int> value);

                void set_uniform(GLint location, vec3_t<float> value);

                void set_uniform(GLint location, vec4_t<bool> value);

                void set_uniform(GLint location, vec4_t<int> value);

                void set_uniform(GLint location, vec4_t<float> value);

                void set_uniform(GLint location, mat2x2_t<float> value);

                void set_uniform(GLint location, mat3x3_t<float> value);

                void set_uniform(GLint location, mat4x4_t<float> value);

                void set_uniform(GLint location, unsigned int texture_unit, const texture_2d &texture);

                void set_uniform(const std::string &name, unsigned int texture_unit, const texture_2d &texture);

                void use() const;

                operator GLuint() const;

            private:
                GLuint gl_object_;
                bool linked_;
                std::vector<std::reference_wrapper<shader>> attached_shaders_;
            };
        }
    }
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_PROGRAM_HPP
