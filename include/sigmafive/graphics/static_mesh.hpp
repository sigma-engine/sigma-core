#ifndef SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_HPP

#include <sigmafive/config.hpp>

#include <array>
#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/vector.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <sigmafive/component.hpp>
#include <sigmafive/reflect/reflect.hpp>
#include <sigmafive/resource/identifier.hpp>
#include <sigmafive/util/glm_serialize.hpp>

namespace sigmafive {
namespace graphics {

    RCLASS()
    class SIGMAFIVE_API static_mesh {
    public:
        struct vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec3 tangent;
            glm::vec2 texcoord;
        };

        typedef std::array<unsigned int, 3> triangle;

        static_mesh() = default;

        static_mesh(const static_mesh&) = delete;

        static_mesh(static_mesh&&) = default;

        static_mesh& operator=(const static_mesh&) = delete;

        static_mesh& operator=(static_mesh&&) = default;

        ~static_mesh() = default;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& vertices;
            ar& triangles;
            ar& material;
        }

        std::vector<vertex> vertices;
        std::vector<triangle> triangles;
        resource::identifier material;
    };

    class SIGMAFIVE_API static_mesh_cache {
    public:
        static_mesh_cache() = default;

        static_mesh_cache(static_mesh_cache&&) noexcept = default;

        static_mesh_cache(const static_mesh_cache&) = delete;

        static_mesh_cache& operator=(static_mesh_cache&&) noexcept = default;

        static_mesh_cache& operator=(const static_mesh_cache&) = delete;

        virtual ~static_mesh_cache() = default;

        /**
        * @brief Returns if mesh is loaded in this cache.
        *
        * @param mesh the mesh to check if cached.
        * @return true if the mesh is cache.
        */
        virtual bool is_cached(resource::identifier mesh) const = 0;

        /**
        * @brief Increases the reference count associated with the mesh.
        *
        *
        * @param mesh the mesh to increase the reference count of.
        * @return true if the mesh exists and is valid.
        */
        virtual bool increment_reference(resource::identifier mesh) = 0;

        /**
        * @brief Decreases the reference count associated with
        * the mesh.
        *
        *
        * @param mesh the mesh to decrease the reference count of.
        * @returns true if the mesh reference count is zero.
        */
        virtual bool decrement_reference(resource::identifier mesh) = 0;
    };
}
}

namespace boost {
namespace serialization {
    template <class Archive>
    void serialize(Archive& ar, sigmafive::graphics::static_mesh::vertex& vertex,
        const unsigned int version)
    {
        ar& vertex.position;
        ar& vertex.normal;
        ar& vertex.tangent;
        ar& vertex.texcoord;
    }
}
}

#endif // SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_HPP
