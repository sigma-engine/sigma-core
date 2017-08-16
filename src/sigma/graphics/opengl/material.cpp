#include <sigma/graphics/opengl/material.hpp>

#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

namespace sigma {
namespace opengl {
    material::material(shader_manager& shader_mgr, const graphics::material& data)
        : technique(shader_mgr, data.technique_id, data)
        , data(data)
    {
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::NORMAL_METALNESS_LOCATION, geometry_buffer::NORMAL_METALNESS_OUTPUT_NAME));
    }
}
}
