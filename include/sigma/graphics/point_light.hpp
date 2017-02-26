#ifndef SIGMA_GRAPHICS_POINT_LIGHT_HPP
#define SIGMA_GRAPHICS_POINT_LIGHT_HPP

#include <glm/vec3.hpp>
#include <sigma/component.hpp>
#include <sigma/config.hpp>

namespace sigma {
namespace graphics {

    struct SIGMA_API point_light {
        float falloff = .25f;
        float intensity = 1.0f;
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    };

    using point_light_manager = basic_component_manager<point_light>;
}
}

#endif // SIGMA_GRAPHICS_POINT_LIGHT_HPP
