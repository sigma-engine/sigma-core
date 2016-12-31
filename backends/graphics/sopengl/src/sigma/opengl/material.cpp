#include <sigma/opengl/material.hpp>

#include <sigma/opengl/geometry_buffer.hpp>
#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {

    void material::link()
    {
        shader_technique::link();

        //GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::WORLD_POSITION_OUTPUT_LOCATION, geometry_buffer::WORLD_POSITION_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::DIFFUSE_COLOR_OUTPUT_LOCATION, geometry_buffer::DIFFUSE_COLOR_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::NORMAL_OUTPUT_LOCATION, geometry_buffer::NORMAL_OUTPUT_NAME));
    }
}
}
