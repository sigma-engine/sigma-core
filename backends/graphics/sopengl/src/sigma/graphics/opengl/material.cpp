#include <sigma/graphics/opengl/material.hpp>

#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/util.hpp>

namespace sigma {
namespace opengl {
    material::material(const graphics::material_data& data)
        : shader_technique<graphics::material>(data)
    {
    }

    void material::link()
    {
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::NORMAL_METALNESS_LOCATION, geometry_buffer::NORMAL_METALNESS_OUTPUT_NAME));
        shader_technique::link();
    }
}
}
