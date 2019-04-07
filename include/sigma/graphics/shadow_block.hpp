#ifndef SIGMA_GRAPHICS_SHADOW_BLOCK_HPP
#define SIGMA_GRAPHICS_SHADOW_BLOCK_HPP

#include <sigma/config.hpp>
#include <sigma/reflect.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <array>

namespace sigma {
namespace graphics {
    struct shadow_block {
        std::array<glm::mat4, 3> light_projection_view_matrix;
        std::array<glm::vec4, 3> light_frustum_far_plane;
    };
}
}

// #include <sigma/graphics/shadow_block.std140.hpp>

#endif // SIGMA_GRAPHICS_SHADOW_BLOCK_HPP
