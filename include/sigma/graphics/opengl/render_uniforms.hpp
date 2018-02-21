#ifndef SIGMA_GRAPHICS_OPENGL_RENDER_UNIFORMS_HPP
#define SIGMA_GRAPHICS_OPENGL_RENDER_UNIFORMS_HPP

#include <sigma/config.hpp>
#include <sigma/reflect.hpp>

#include <glm/matrix.hpp>

namespace sigma {
namespace opengl {
    struct instance_matrices {
        BOOST_HANA_DEFINE_STRUCT(
            instance_matrices,
            (glm::mat4, model_matrix),
            (glm::mat4, model_view_matrix),
            (glm::mat3, normal_matrix));
    };

    struct directional_light_block {
        BOOST_HANA_DEFINE_STRUCT(
            directional_light_block,
            (glm::vec4, color_intensity),
            (glm::vec4, direction_layer));
    };

    struct point_light_block {
        BOOST_HANA_DEFINE_STRUCT(
            point_light_block,
            (glm::vec4, color_intensity),
            (glm::vec4, position_radius));
    };

    struct spot_light_block {
        BOOST_HANA_DEFINE_STRUCT(
            spot_light_block,
            (glm::vec4, color_intensity),
            (glm::vec4, position_cutoff),
            (glm::vec4, direction_layer));
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_RENDER_UNIFORMS_HPP
