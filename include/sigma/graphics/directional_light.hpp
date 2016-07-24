#ifndef SIGMA_ENGINE_GRAPHICS_DIRECTIONAL_LIGHT_HPP
#define SIGMA_ENGINE_GRAPHICS_DIRECTIONAL_LIGHT_HPP

#include <sigma/config.hpp>

#include <sigma/component.hpp>
#include <sigma/reflect/reflect.hpp>

namespace sigma {
namespace graphics {

    RCLASS()
    struct sigma_API directional_light {
    };

    using directional_light_manager = basic_component_manager<directional_light>;
}
}

SIGMA_COMPONENT(sigma::graphics::directional_light)

#endif // SIGMA_ENGINE_GRAPHICS_DIRECTIONAL_LIGHT_HPP
