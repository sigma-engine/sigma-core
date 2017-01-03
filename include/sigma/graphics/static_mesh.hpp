#ifndef SIGMA_GRAPHICS_STATIC_MESH_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_HPP

#include <sigma/config.hpp>
#include <sigma/resource/manager.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <boost/serialization/array.hpp>
#include <boost/serialization/vector.hpp>

#include <array>
#include <vector>

namespace sigma {
namespace graphics {

    struct static_mesh_data {
        struct vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec3 tangent;
            glm::vec2 texcoord;
            template <class Archive>
            void serialize(Archive& ar, const unsigned int version)
            {
                ar& position;
                ar& normal;
                ar& tangent;
                ar& texcoord;
            }
        };
        typedef std::array<unsigned int, 3> triangle;

        std::vector<vertex> vertices;
        std::vector<triangle> triangles;
        resource::identifier material;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& vertices;
            ar& triangles;
            ar& material;
        }
    };

    class SIGMA_API static_mesh {
    public:
        using resource_data = static_mesh_data;

        static_mesh() = default;

        static_mesh(static_mesh&&) = default;

        static_mesh& operator=(static_mesh&&) = default;

        virtual ~static_mesh() = default;

    private:
        static_mesh(const static_mesh&) = delete;
        static_mesh& operator=(const static_mesh&) = delete;
    };

    using static_mesh_manager = resource::manager<static_mesh>;
}
}

#endif // SIGMA_GRAPHICS_STATIC_MESH_HPP
