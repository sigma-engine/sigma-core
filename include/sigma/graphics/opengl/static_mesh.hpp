#ifndef SIGMA_ENGINE_STATIC_MESH_HPP
#define SIGMA_ENGINE_STATIC_MESH_HPP

#include <sigma/graphics/static_mesh.hpp>

#include <sigma/config.hpp>
#include <sigma/graphics/opengl/material.hpp>
#include <sigma/resource/cache.hpp>

#include <glad/glad.h>

#include <cstddef>

#define STATIC_MESH_PTR(static_mesh_mgr, x) static_mesh_mgr.acquire(x)

namespace sigma {
namespace opengl {
    class static_mesh {
    public:
        static_mesh(material_manager& material_mgr, const graphics::static_mesh& data);

        static_mesh(static_mesh&&) = default;

        static_mesh& operator=(static_mesh&&) = default;

        ~static_mesh();

        void render(unsigned int material_slot) const;

        void render_all() const;

        const graphics::static_mesh& data;

        // TODO make private
        GLuint vertex_array_ = 0;
        GLuint vertex_buffer_ = 0;
        GLuint index_buffer_ = 0;
        GLsizei index_count_ = 0;

    private:
        static_mesh(const static_mesh&) = delete;
        static_mesh& operator=(const static_mesh&) = delete;
    };

    class static_mesh_manager {
    public:
        // TODO remove the use of unique_ptr

        static_mesh_manager(material_manager& materials, resource::cache<graphics::static_mesh>& static_mesh_cache)
            : materials_(materials)
            , static_mesh_cache_(static_mesh_cache)
        {
        }

        opengl::static_mesh* acquire(const resource::handle<graphics::static_mesh>& hndl)
        {
            // TODO not thread safe

            auto data = static_mesh_cache_.acquire(hndl);
            if (hndl.index >= static_meshes_.size())
                static_meshes_.resize(hndl.index + 1);

            if (static_meshes_[hndl.index] == nullptr)
                static_meshes_[hndl.index] = std::make_unique<static_mesh>(materials_, *data);

            return static_meshes_.at(hndl.index).get();
        }

    private:
        material_manager& materials_;
        resource::cache<graphics::static_mesh>& static_mesh_cache_;
        std::vector<std::unique_ptr<static_mesh>> static_meshes_;
    };
}
}

#endif // SIGMA_ENGINE_STATIC_MESH_HPP
