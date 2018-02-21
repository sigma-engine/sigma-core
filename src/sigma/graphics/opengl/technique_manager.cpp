#include <sigma/graphics/opengl/technique_manager.hpp>

#include <sigma/graphics/opengl/cubemap_manager.hpp>
#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/shader_manager.hpp>
#include <sigma/graphics/standard_block.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace sigma {
namespace opengl {
    technique_manager::technique_manager(resource::cache<graphics::technique>& technique_cache, shader_manager& shaders)
        : technique_cache_(technique_cache)
        , shaders_(shaders)
    {
    }

    technique_manager::~technique_manager()
    {
        for (auto program : programs_)
            glDeleteProgram(program);
    }

    std::pair<graphics::technique*, GLuint> technique_manager::acquire(const resource::handle<graphics::technique>& hndl)
    {
        assert(hndl.is_valid());
        if (hndl.index >= programs_.size())
            programs_.resize(hndl.index + 1, 0);

        auto data = technique_cache_.acquire(hndl);

        if (programs_[hndl.index] == 0) {
            programs_[hndl.index] = glCreateProgram();

            glAttachShader(programs_[hndl.index], shaders_.acquire(data->vertex).second);

            if (data->tessellation_control.is_valid())
                glAttachShader(programs_[hndl.index], shaders_.acquire(data->tessellation_control).second);

            if (data->tessellation_evaluation.is_valid())
                glAttachShader(programs_[hndl.index], shaders_.acquire(data->tessellation_evaluation).second);

            if (data->geometry.is_valid())
                glAttachShader(programs_[hndl.index], shaders_.acquire(data->geometry).second);

            glAttachShader(programs_[hndl.index], shaders_.acquire(data->fragment).second);

            glLinkProgram(programs_[hndl.index]);

            GLint linkded = GL_FALSE;
            glGetProgramiv(programs_[hndl.index], GL_LINK_STATUS, &linkded);
            if (linkded == GL_FALSE) {
                GLint length = 0;
                glGetProgramiv(programs_[hndl.index], GL_INFO_LOG_LENGTH, &length);

                // The length includes the NULL character
                std::vector<GLchar> error_buffer(length);
                glGetProgramInfoLog(programs_[hndl.index], length, &length, error_buffer.data());

                std::string error_string(error_buffer.begin(), error_buffer.end());
                std::cerr << error_string << '\n';

                std::abort(); // TODO be more subtle???
            }

            glUseProgram(programs_[hndl.index]);

            GLint in_image_loc = glGetUniformLocation(programs_[hndl.index], geometry_buffer::IMAGE_INPUT_NAME);
            GLint in_diffuse_loc = glGetUniformLocation(programs_[hndl.index], geometry_buffer::DIFFUSE_ROUGHNESS_INPUT_NAME);
            GLint in_normal_loc = glGetUniformLocation(programs_[hndl.index], geometry_buffer::NORMAL_METALNESS_INPUT_NAME);
            GLint in_depth_stencil_loc = glGetUniformLocation(programs_[hndl.index], geometry_buffer::DEPTH_STENCIL_INPUT_NAME);
            GLint in_shadow_map_loc = glGetUniformLocation(programs_[hndl.index], geometry_buffer::SHADOW_MAP_NAME);

            glUniform1i(in_image_loc, geometry_buffer::INPUT_IMAGE_LOCATION);
            glUniform1i(in_diffuse_loc, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION);
            glUniform1i(in_normal_loc, geometry_buffer::NORMAL_METALNESS_LOCATION);
            glUniform1i(in_depth_stencil_loc, geometry_buffer::DEPTH_STENCIL_LOCATION);
            glUniform1i(in_shadow_map_loc, geometry_buffer::SHADOW_MAP_LOCATION);
        }

        return { data, programs_[hndl.index] };
    }
}
}
