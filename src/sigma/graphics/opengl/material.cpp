#include <sigma/graphics/opengl/material.hpp>

#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

namespace sigma {
namespace opengl {
    material::material(const graphics::material& data)
        : shader_technique<graphics::material>(data)
    {
    }

    void material::link(texture_manager& texture_mgr,cubemap_manager& cubemap_mgr, shader_manager& shader_mgr)
    {
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::NORMAL_METALNESS_LOCATION, geometry_buffer::NORMAL_METALNESS_OUTPUT_NAME));
        shader_technique::link(texture_mgr, cubemap_mgr, shader_mgr);
    }
}
}
