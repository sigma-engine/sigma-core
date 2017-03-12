#include <sigma/graphics/opengl/shader.hpp>

#include <sigma/graphics/opengl/util.hpp>

#include <cassert>
#include <iostream>
#include <vector>

namespace sigma {
namespace opengl {
    shader_type convert(graphics::shader_type type)
    {
        switch (type) {
        case graphics::shader_type::vertex:
            return shader_type::VERTEX_SHADER;
        case graphics::shader_type::fragment:
            return shader_type::FRAGMENT_SHADER;
        case graphics::shader_type::geometry:
            return shader_type::GEOMETRY_SHADER;
        }
    }

    shader::shader(shader_type type, std::string source)
    {
        const char* src = source.c_str();

        GL_CHECK(object_ = glCreateShader(GLenum(type)));
        GL_CHECK(glShaderSource(object_, 1, &src, nullptr));
        GL_CHECK(glCompileShader(object_));

        GLint compiled;
        glGetShaderiv(object_, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(object_, GL_INFO_LOG_LENGTH, &length);

            // The length includes the NULL character
            std::vector<GLchar> error_buffer(length);
            glGetShaderInfoLog(object_, length, &length, error_buffer.data());

            std::cout << src << '\n';
            std::string error_string(error_buffer.begin(), error_buffer.end());
            std::cout << error_string << '\n';

            std::abort();
        }
    }

    shader::shader(graphics::shader_data data)
        : shader(convert(data.type), data.source)
    {
    }

    shader::~shader()
    {
        glDeleteShader(object_);
    }

    GLuint shader::get_object() const
    {
        return object_;
    }

    std::unique_ptr<graphics::shader> shader_manager::create(graphics::shader_data data)
    {
        return std::make_unique<shader>(std::move(data));
    }
}
}
