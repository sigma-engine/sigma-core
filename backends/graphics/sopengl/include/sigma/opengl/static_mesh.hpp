#ifndef sigma_GRAPHICS_OPENGL_STATIC_MESH_HPP
#define sigma_GRAPHICS_OPENGL_STATIC_MESH_HPP

#include <sigma/opengl/config.hpp>

#include <sigma/graphics/static_mesh.hpp>

#include <unordered_map>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <sigma/graphics/renderer.hpp>
#include <sigma/opengl/index_buffer.hpp>
#include <sigma/opengl/material.hpp>
#include <sigma/opengl/vertex_array.hpp>
#include <sigma/opengl/vertex_buffer.hpp>

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

#endif // sigma_GRAPHICS_OPENGL_STATIC_MESH_HPP
