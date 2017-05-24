// TEMPLATE(generated.hpp)
// TEMPLATE(std140.hpp)
// TEMPLATE(glsl)
#ifndef SIGMA_GRAPHICS_SHADOW_BLOCK_HPP
#define SIGMA_GRAPHICS_SHADOW_BLOCK_HPP

#include <sigma/reflect.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct R_EXPORT(std140, binding = 1) shadow_block {
        glm::mat4 light_projection_view_matrix[3];
        glm::vec4 light_frustum_far_plane[3];
    };
}
}

#include <sigma/graphics/shadow_block.generated.hpp>
#include <sigma/graphics/shadow_block.std140.hpp>

#endif // SIGMA_GRAPHICS_SHADOW_BLOCK_HPP
