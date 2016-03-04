#ifndef SIGMAFIVE_ENGINE_GRAPHICS_DIRECTIONAL_LIGHT_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_DIRECTIONAL_LIGHT_HPP

#include <sigmafive/config.hpp>

#include <sigmafive/component.hpp>

namespace sigmafive {
namespace graphics {
    struct SIGMAFIVE_API directional_light {
        bool visible; // TODO check the odering on this
    };

    using directional_light_manager = basic_component_manager<directional_light>;
}
}

SIGMAFIVE_COMPONENT(sigmafive::graphics::directional_light)

#endif //SIGMAFIVE_ENGINE_GRAPHICS_DIRECTIONAL_LIGHT_HPP
