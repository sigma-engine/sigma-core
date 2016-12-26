#ifndef SIGMA_GRAPHICS_STATIC_MESH_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_HPP

#include <sigma/component.hpp>
#include <sigma/config.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/reflect/reflect.hpp>
#include <sigma/resource/identifier.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/vector.hpp>

#include <array>
#include <utility>
#include <vector>

namespace sigma {
namespace graphics {

    RCLASS()
    class SIGMA_API static_mesh {
    public:
        struct vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec3 tangent;
            glm::vec2 texcoord;
        };

        typedef std::array<unsigned int, 3> triangle;

        static_mesh() = default;

        static_mesh(static_mesh&&) = default;

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

        friend class static_mesh_cache;
        std::size_t reference_count = 0;

    private:
        static_mesh(const static_mesh&) = delete;
        static_mesh& operator=(const static_mesh&) = delete;
    };

    class SIGMA_API static_mesh_cache : public resource::resource_cache<static_mesh> {
    public:
        static_mesh_cache(boost::filesystem::path cache_directory, material_cache& materials);

        virtual bool increment_reference(resource::identifier resource_id) override;

        virtual bool decrement_reference(resource::identifier resource_id) override;

    private:
        material_cache& materials_;
    };
}
}

namespace boost {
namespace serialization {
    template <class Archive>
    void serialize(Archive& ar, sigma::graphics::static_mesh::vertex& vertex,
        const unsigned int version)
    {
        ar& vertex.position;
        ar& vertex.normal;
        ar& vertex.tangent;
        ar& vertex.texcoord;
    }
}
}

#endif // SIGMA_GRAPHICS_STATIC_MESH_HPP
