#ifndef SIGMA_GRAPHICS_STATIC_MESH_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/resource/cache.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <boost/serialization/array.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

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
            void serialize(Archive& ar, const unsigned int version)
            {
                ar& position;
                ar& normal;
                ar& bitangent;
                ar& tangent;
                ar& texcoord;
            }
        };
        typedef std::array<unsigned int, 3> triangle;

        std::vector<vertex> vertices;
        std::vector<triangle> triangles;
        std::vector<resource::handle<material>> materials;
        std::vector<std::pair<std::size_t, std::size_t>> material_slots;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& vertices;
            ar& triangles;
            ar& materials;
            ar& material_slots;
        }
    };
}
}

BOOST_CLASS_VERSION(sigma::graphics::static_mesh, 1);

#endif // SIGMA_GRAPHICS_STATIC_MESH_HPP
