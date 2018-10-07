#ifndef SIGMA_GRAPHICS_STATIC_MESH_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/util/filesystem.hpp>

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
        };
        typedef std::array<unsigned int, 3> triangle;

        float radius = 0;
        std::vector<vertex> vertices;
        std::vector<triangle> triangles;
        std::vector<std::shared_ptr<material>> materials;
        std::vector<std::pair<std::size_t, std::size_t>> material_slots;
    };
}
}

#endif // SIGMA_GRAPHICS_STATIC_MESH_HPP
