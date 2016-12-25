#ifndef SIGMA_GRAPHICS_OPENGL_STATIC_MESH_HPP
#define SIGMA_GRAPHICS_OPENGL_STATIC_MESH_HPP

#include <sigma/opengl/config.hpp>
#include <sigma/opengl/gl_core_4_0.h>

#include <sigma/resource/identifier.hpp>

namespace sigma {
namespace opengl {
    struct static_mesh {
        GLuint vertex_array = 0;
        GLuint vertex_buffer = 0;
        GLuint index_buffer = 0;
        GLuint index_count = 0;
        resource::identifier material;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_STATIC_MESH_HPP
