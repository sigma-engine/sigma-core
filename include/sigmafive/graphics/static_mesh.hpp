#ifndef SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_HPP

#include <sigmafive/config.hpp>

#include <array>
#include <boost/serialization/access.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/vector.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <sigmafive/component.hpp>
#include <sigmafive/resource/identifier.hpp>
#include <sigmafive/util/glm_serialize.hpp>
#include <vector>

namespace sigmafive {
namespace graphics {
    class SIGMAFIVE_API static_mesh {
    public:
        struct vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec3 tangent;
            glm::vec2 texcoord;
        };

        typedef std::array<unsigned int, 3> triangle;

        void set_data(std::vector<vertex> vertices, std::vector<triangle> triangles);

        const std::vector<vertex>& vertices() const;

        const std::vector<triangle>& triangles() const;

        resource::identifier get_material() const;

        void set_material(resource::identifier material);

    private:
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& vertices_;
            ar& triangles_;
            ar& material_;
        }

        std::vector<vertex> vertices_;
        std::vector<triangle> triangles_;
        resource::identifier material_;
    };

    class static_mesh_cache {
    public:
        static_mesh_cache() = default;

        static_mesh_cache(const static_mesh_cache&) = delete;

        virtual ~static_mesh_cache() = default;

        static_mesh_cache& operator=(const static_mesh_cache&) = delete;

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
        * NOTE calling this method will not invalidate any references to other meshes
        * in this cache.
        *
        * @param mesh the mesh to increase the reference count of.
        * @return true if the mesh exists and is valid.
        */
        virtual bool increment_reference(resource::identifier mesh) = 0;

        /**
        * @brief Decreases the reference count associated with
        * the mesh.
        *
        * NOTE calling this method wiil not invalidate any references to other meshes
        * in this cache.
        *
        * @param mesh the mesh to decrease the reference count of.
        * @returns true if the mesh reference count is zero.
        */
        virtual bool decrement_reference(resource::identifier mesh) = 0;

        //virtual static_mesh& get(resource::identifier mesh) = 0;

        //virtual const static_mesh& add(resource::identifier mesh) const = 0;
    };
}
}

namespace boost {
namespace serialization {
    template <class Archive>
    void serialize(Archive& ar, sigmafive::graphics::static_mesh::vertex& vertex, const unsigned int version)
    {
        ar& vertex.position;
        ar& vertex.normal;
        ar& vertex.tangent;
        ar& vertex.texcoord;
    }
}
}

#endif //SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_HPP
