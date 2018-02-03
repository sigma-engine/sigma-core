#ifndef SIGMA_ENGINE_STATIC_MESH_MANAGER_HPP
#define SIGMA_ENGINE_STATIC_MESH_MANAGER_HPP

#include <sigma/graphics/static_mesh.hpp>

#include <sigma/buddy_array_allocator.hpp>
#include <sigma/config.hpp>
#include <sigma/resource/cache.hpp>

#include <glad/glad.h>

#include <cstddef>

#define VERTEX_BLOCK_SIZE 64
#define MAX_VERTEX_BLOCKS 512

#define INDEX_BLOCK_SIZE 63
#define MAX_INDEX_BLOCKS 1040

namespace sigma {
namespace opengl {
    class static_mesh_manager {
    public:
        struct mesh_buffer {
            std::size_t batch_index = -1;
            std::size_t base_vertex = 0;
            std::size_t base_index = 0;
        };

        static_mesh_manager(resource::cache<graphics::static_mesh>& static_mesh_cache);

        static_mesh_manager(static_mesh_manager&&) = default;

        static_mesh_manager& operator=(static_mesh_manager&&) = default;

        ~static_mesh_manager();

        std::size_t batch_size() const;

        void bind_batch(std::size_t batch);

        std::pair<graphics::static_mesh*, mesh_buffer> acquire(const resource::handle<graphics::static_mesh>& hndl);

    private:
        static_mesh_manager(const static_mesh_manager&) = delete;
        static_mesh_manager& operator=(const static_mesh_manager&) = delete;

        resource::cache<graphics::static_mesh>& static_mesh_cache_;

        std::vector<mesh_buffer> static_meshes_;

        struct buffer_batch {
            GLuint vertex_array = 0;

            // MAX_VERTEX_BLOCKS blocks of VERTEX_BLOCK_SIZE vertices.
            buddy_array_allocator vertex_allocator{ MAX_VERTEX_BLOCKS };
            GLuint vertex_buffer = 0;

            // MAX_INDEX_BLOCKS blocks of INDEX_BLOCK_SIZE indicies.
            buddy_array_allocator index_allocator{ MAX_INDEX_BLOCKS };
            GLuint index_buffer = 0;
        };

        std::vector<buffer_batch> batches_;
    };
}
}

#endif // SIGMA_ENGINE_STATIC_MESH_MANAGER_HPP
