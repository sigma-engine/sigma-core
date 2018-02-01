#ifndef SIGMA_ENGINE_STATIC_MESH_MANAGER_HPP
#define SIGMA_ENGINE_STATIC_MESH_MANAGER_HPP

#include <sigma/graphics/static_mesh.hpp>

#include <sigma/config.hpp>
#include <sigma/resource/cache.hpp>

#include <glad/glad.h>

#include <cstddef>

namespace sigma {
namespace opengl {
    class static_mesh_manager {
    public:
        struct mesh_buffer {
            GLuint vertex_array = 0;
            GLuint vertex_buffer = 0;
            GLuint index_buffer = 0;
            GLsizei index_count = 0;
        };

        static_mesh_manager(resource::cache<graphics::static_mesh>& static_mesh_cache);

        static_mesh_manager(static_mesh_manager&&) = default;

        static_mesh_manager& operator=(static_mesh_manager&&) = default;

        ~static_mesh_manager();

        mesh_buffer acquire(const resource::handle<graphics::static_mesh>& hndl);

    private:
        static_mesh_manager(const static_mesh_manager&) = delete;
        static_mesh_manager& operator=(const static_mesh_manager&) = delete;

        resource::cache<graphics::static_mesh>& static_mesh_cache_;
        std::vector<mesh_buffer> static_meshes_;
    };
}
}

#endif // SIGMA_ENGINE_STATIC_MESH_MANAGER_HPP
