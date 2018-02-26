#include <sigma/graphics/opengl/shader_manager.hpp>

#include <sigma/graphics/opengl/util.hpp>
#include <sigma/graphics/shader.hpp>

#include <glad/glad.h>

#include <iostream>
#include <vector>

namespace sigma {
namespace opengl {
    GLenum convert(graphics::shader_type type)
    {
        switch (type) {
        case graphics::shader_type::vertex:
            return GL_VERTEX_SHADER;
        case graphics::shader_type::fragment:
            return GL_FRAGMENT_SHADER;
        case graphics::shader_type::geometry:
            return GL_GEOMETRY_SHADER;
        case graphics::shader_type::tessellation_control:
            return GL_TESS_CONTROL_SHADER;
        case graphics::shader_type::tessellation_evaluation:
            return GL_TESS_EVALUATION_SHADER;
        case graphics::shader_type::header:
            return GL_NONE;
        }
        return GL_NONE;
    }

    shader_manager::shader_manager(resource::cache<graphics::shader>& shader_cache)
        : shader_cache_(shader_cache)
    {
    }

    shader_manager::~shader_manager()
    {
        for (auto shader : shaders_)
            glDeleteShader(shader);
    }

    std::pair<graphics::shader*, GLuint> shader_manager::acquire(const resource::handle<graphics::shader>& hndl)
    {
        assert(hndl.is_valid());
        if (hndl.index >= shaders_.size())
            shaders_.resize(hndl.index + 1, 0);

        auto data = shader_cache_.acquire(hndl);
        if (shaders_[hndl.index] == 0) {
            shaders_[hndl.index] = glCreateShader(convert(data->type));
            glShaderBinary(1, &shaders_[hndl.index], GL_SHADER_BINARY_FORMAT_SPIR_V, data->spirv.data(), data->spirv.size());
            glSpecializeShader(shaders_[hndl.index], "main", 0, nullptr, nullptr);

            GLint compiled;
            glGetShaderiv(shaders_[hndl.index], GL_COMPILE_STATUS, &compiled);
            if (compiled == GL_FALSE) {
                GLint length = 0;
                glGetShaderiv(shaders_[hndl.index], GL_INFO_LOG_LENGTH, &length);

                // The length includes the NULL character
                std::vector<GLchar> error_buffer(length);
                glGetShaderInfoLog(shaders_[hndl.index], length, &length, error_buffer.data());

                std::string error_string(error_buffer.begin(), error_buffer.end());
                std::cerr << error_string << '\n';

                std::abort(); // TODO be more subtle???
            }
        }

        return { data, shaders_[hndl.index] };
    }
}
}
