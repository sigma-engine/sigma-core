#ifndef SIGMA_ENGINE_STATIC_MESH_HPP
#define SIGMA_ENGINE_STATIC_MESH_HPP

#include <sigma/opengl/gl_core_4_2.h>

#include <sigma/graphics/static_mesh.hpp>

#include <memory>
#include <vector>

namespace sigma {
namespace opengl {
    struct render_matrices;
    class material;
    class static_mesh {
    public:
        static_mesh(const std::vector<graphics::static_mesh::vertex>& vertices, const std::vector<graphics::static_mesh::triangle>& triangles, std::shared_ptr<material> mat);

        ~static_mesh();

        void render(render_matrices* matrices);

    private:
        static_mesh(const static_mesh&) = delete;
        static_mesh& operator=(const static_mesh&) = delete;

        GLuint vertex_array_ = 0;
        GLuint vertex_buffer_ = 0;
        GLuint index_buffer_ = 0;
        GLuint index_count_ = 0;
        std::shared_ptr<material> material_ = nullptr;
    };
}
}

#endif // SIGMA_ENGINE_STATIC_MESH_HPP
