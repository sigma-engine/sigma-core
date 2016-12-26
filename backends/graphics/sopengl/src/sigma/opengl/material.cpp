#include <sigma/opengl/material.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {

    void material::link()
    {
        shader_technique::link();

        GL_CHECK(glBindFragDataLocation(object_, WORLD_POSITION_OUTPUT_LOCATION, WORLD_POSITION_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, DIFFUSE_COLOR_OUTPUT_LOCATION, DIFFUSE_COLOR_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, NORMAL_OUTPUT_LOCATION, NORMAL_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, TEXTURE_COORDINATE_OUTPUT_LOCATION, TEXTURE_COORDINATE_OUTPUT_NAME));
    }
}
}
