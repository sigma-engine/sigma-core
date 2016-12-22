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
    class static_mesh : public graphics::static_mesh {
    public:
        static_mesh() = default;

        static_mesh(const static_mesh&) = delete;

        static_mesh(static_mesh&&) = default;

        static_mesh& operator=(const static_mesh&) = delete;

        static_mesh& operator=(static_mesh&&) = default;

        ~static_mesh() = default;

        GLuint vertex_array = 0;
        GLuint vertex_buffer = 0;
        GLuint index_buffer = 0;
        std::size_t reference_count = 0;
    };

    class static_mesh_cache
        : public graphics::static_mesh_cache {
    public:
        static_mesh_cache(material_cache& materials);

        static_mesh_cache(const static_mesh_cache&) = delete;

        static_mesh_cache(static_mesh_cache&&) noexcept = default;

        static_mesh_cache& operator=(const static_mesh_cache&) = delete;

        static_mesh_cache& operator=(static_mesh_cache&&) = default;

        virtual ~static_mesh_cache();

        virtual bool is_cached(resource::identifier mesh) const override;

        virtual bool increment_reference(resource::identifier mesh) override;

        virtual bool decrement_reference(resource::identifier mesh) override;

        void update();

        void render(resource::identifier mesh_id, resource::identifier material_id,
            const glm::mat4 projection_matrix, const glm::mat4 view_matrix,
            const glm::mat4 model_matrix) const;

        void render(resource::identifier mesh_id, const glm::mat4 projection_matrix,
            const glm::mat4 view_matrix, const glm::mat4 model_matrix) const;

    public:
        using size_type = std::vector<static_mesh>::size_type;
        using difference_type = std::vector<static_mesh>::difference_type;

        boost::filesystem::path cache_directory_;
        std::unordered_map<resource::constexpr_identifier, size_type> resource_map_;

        material_cache& materials_;
        std::vector<static_mesh> static_meshes_;
        size_type dirty_;
    };
}
}

#endif // sigma_GRAPHICS_OPENGL_STATIC_MESH_HPP
