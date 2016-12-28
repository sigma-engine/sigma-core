#ifndef SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP
#define SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP

#include <sigma/component.hpp>
#include <sigma/config.hpp>
#include <sigma/reflect/reflect.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {

    RCLASS()
    struct SIGMA_API directional_light {
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        float intensity = 1.0f;
    };

    using directional_light_manager = basic_component_manager<directional_light>;
}
}

#endif // SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP
