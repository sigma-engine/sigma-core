#ifndef SIGMA_ENGINE_OPENGL_PROGRAM_HPP
#define SIGMA_ENGINE_OPENGL_PROGRAM_HPP

#include <functional>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <sigma/opengl/gl_core_4_0.h>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/texture_2d.hpp>
#include <string>
#include <vector>

namespace sigma {
namespace opengl {
    /*class SIGMA_ENGINE_OPENGL_API program {
public:
program();

~program();

bool is_linked() const;

void attach(shader& shader);

void detach(shader& shader);

std::string link();

GLint get_uniform_location(std::string name) const;

template <typename T>
void set_uniform(const std::string& name, T&& value)
{
    set_uniform(get_uniform_location(name), std::forward<T>(value));
}

void set_uniform(GLint location, bool value);

void set_uniform(GLint location, int value);

void set_uniform(GLint location, float value);

void set_uniform(GLint location, glm::bvec2 value);

void set_uniform(GLint location, glm::ivec2 value);

void set_uniform(GLint location, glm::vec2 value);

void set_uniform(GLint location, glm::bvec3 value);

void set_uniform(GLint location, glm::ivec3 value);

void set_uniform(GLint location, glm::vec3 value);

void set_uniform(GLint location, glm::bvec4 value);

void set_uniform(GLint location, glm::ivec4 value);

void set_uniform(GLint location, glm::vec4 value);

void set_uniform(GLint location, glm::mat2 value);

void set_uniform(GLint location, glm::mat3 value);

void set_uniform(GLint location, glm::mat4 value);

void use() const;

operator GLuint() const;

private:
GLuint gl_object_;
bool linked_;
std::vector<std::reference_wrapper<shader> > attached_shaders_;
};*/
}
}

#endif // SIGMA_ENGINE_OPENGL_PROGRAM_HPP
