#ifndef SIGMA_GRAPHICS_STATIC_MESH_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/resource/cache.hpp>
#include <sigma/resource/resource.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace sigma {
namespace graphics {
    struct static_mesh {
        struct vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec3 bitangent;
            glm::vec3 tangent;
            glm::vec2 texcoord;
            template <class Archive>
            void serialize(Archive& ar)
            {
                ar(position,
                normal,
                bitangent,
                tangent,
                texcoord);
            }
        };
        typedef std::array<unsigned int, 3> triangle;

        float radius = 0;
        std::vector<vertex> vertices;
        std::vector<triangle> triangles;
        std::vector<resource::handle<material>> materials;
        std::vector<std::pair<std::size_t, std::size_t>> material_slots;

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(radius,
            vertices,
            triangles,
            materials,
            material_slots);
        }
    };
}
}

REGISTER_RESOURCE(sigma::graphics::static_mesh, static_mesh, 1)

#endif // SIGMA_GRAPHICS_STATIC_MESH_HPP
