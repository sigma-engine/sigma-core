#include <sigma/opengl/program.hpp>

#include <algorithm>
#include <numeric>

namespace sigma {
namespace opengl {
    /*program::program()
    : gl_object_(2)
    , linked_(false)
{
    gl_object_ = glCreateProgram(); //TODO GL_CHECK_ERROR;
}

program::~program()
{
    glDeleteProgram(gl_object_); //TODO GL_CHECK_ERROR;
}

bool program::is_linked() const
{
    return linked_ && std::all_of(begin(attached_shaders_),
end(attached_shaders_),
                          std::function<bool(const
shader&)>(&shader::is_compiled));
}

void program::attach(shader& s)
{
    attached_shaders_.emplace_back(s);
    linked_ = false;
}

void program::detach(shader& s)
{
    attached_shaders_.erase(std::remove_if(begin(attached_shaders_),
end(attached_shaders_),
                                std::bind(std::equal_to<const shader&>(),
std::placeholders::_1,
                                               s)),
        attached_shaders_.end());
    linked_ = false;
}

std::string program::link()
{
    if (!is_linked()) {
        //compile and accumulate the shader error messages
        auto message = std::accumulate(begin(attached_shaders_),
end(attached_shaders_), std::string(""),
            [](std::string message, shader& s) {
                                               return message +
                                                      (message.size() > 0 ? "\n"
: "") +
                                                      (s.is_compiled() ? ""
                                                                       :
s.compile()); //TODO some how get filepath
            });
        if (message.size() > 0)
            return message;

        //attach the shaders to the program object
        std::for_each(begin(attached_shaders_), end(attached_shaders_),
            [=](shader& s) {
                              glAttachShader(gl_object_, s); //TODO
GL_CHECK_ERROR;
            });

        glLinkProgram(gl_object_); //TODO GL_CHECK_ERROR;
        GLint linked;
        glGetProgramiv(gl_object_, GL_LINK_STATUS, &linked); //TODO
GL_CHECK_ERROR;
        linked_ = (linked == GL_TRUE);

        //detach the shaders from the program object
        std::for_each(begin(attached_shaders_), end(attached_shaders_),
[=](shader& s) {
            glDetachShader(gl_object_, s); //TODO GL_CHECK_ERROR;
        });

        if (!linked_) {
            int log_length = 0;
            glGetProgramiv(linked_, GL_INFO_LOG_LENGTH, &log_length); //TODO
GL_CHECK_ERROR;

            std::string message;
            message.resize(log_length);

            glGetProgramInfoLog(gl_object_, log_length, &log_length,
&message[0]); //TODO GL_CHECK_ERROR;

            return message;
        }
    }
    return "";
}

GLint program::get_uniform_location(std::string name) const
{
    return glGetUniformLocation(gl_object_, name.c_str()); //TODO
GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, bool value)
{
    glUniform1ui(location, value); //TODO GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, int value)
{
    glUniform1i(location, value); //TODO GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, float value)
{
    glUniform1f(location, value); //TODO GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::bvec2 value)
{
    glUniform2ui(location, value.x, value.y); //TODO GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::ivec2 value)
{
    glUniform2i(location, value.x, value.y); //TODO GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::vec2 value)
{
    glUniform2f(location, value.x, value.y); //TODO GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::bvec3 value)
{
    glUniform3ui(location, value.x, value.y, value.z); //TODO GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::ivec3 value)
{
    glUniform3i(location, value.x, value.y, value.z); //TODO GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::vec3 value)
{
    glUniform3f(location, value.x, value.y, value.z); //TODO GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::bvec4 value)
{
    glUniform4ui(location, value.x, value.y, value.z, value.w); //TODO
GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::ivec4 value)
{
    glUniform4i(location, value.x, value.y, value.z, value.w); //TODO
GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::vec4 value)
{
    glUniform4f(location, value.x, value.y, value.z, value.w); //TODO
GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::mat2 value)
{
    glUniformMatrix2fv(location, 1, GL_FALSE, &value[0].x); //TODO
GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::mat3 value)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0].x); //TODO
GL_CHECK_ERROR;
}

void program::set_uniform(GLint location, glm::mat4 value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0].x); //TODO
GL_CHECK_ERROR;
}

void program::use() const
{
    glUseProgram(gl_object_);
}

program::operator GLuint() const
{
    return gl_object_;
}*/
}
}
