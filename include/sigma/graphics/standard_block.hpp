#ifndef SIGMA_GRAPHICS_STANDARD_BLOCK_HPP
#define SIGMA_GRAPHICS_STANDARD_BLOCK_HPP

#include <sigma/config.hpp>
#include <sigma/reflect.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct R_EXPORT() standard_block {
        glm::mat4 projection_matrix;
        glm::mat4 inverse_projection_matrix;
        glm::mat4 view_matrix;
        glm::mat4 inverse_view_matrix;
        glm::mat4 projection_view_matrix;
        glm::mat4 inverse_projection_view_matrix;
        glm::vec4 eye_position;
        glm::vec2 view_port_size;
        float time;
        float fovy;
        float z_near;
        float z_far;
        float pad[2];
    };
}
}

// #include <sigma/graphics/standard_block.std140.hpp>

#endif // SIGMA_GRAPHICS_STANDARD_BLOCK_HPP
