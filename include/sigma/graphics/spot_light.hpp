#ifndef SIGMA_ENGINE_GRAPHICS_SPOT_LIGHT_HPP
#define SIGMA_ENGINE_GRAPHICS_SPOT_LIGHT_HPP

#include <sigma/config.hpp>

#include <sigma/component.hpp>
#include <sigma/reflect/reflect.hpp>

namespace sigma {
namespace graphics {

    RCLASS()
    struct sigma_API spot_light {
    };

    using spot_light_manager = basic_component_manager<spot_light>;
}
}

sigma_COMPONENT(sigma::graphics::spot_light)

#endif // SIGMA_ENGINE_GRAPHICS_SPOT_LIGHT_HPP
