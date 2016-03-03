#ifndef SIGMAFIVE_ENGINE_GRAPHICS_SPOT_LIGHT_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_SPOT_LIGHT_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/component.hpp>

namespace sigmafive {
    namespace graphics {
        struct SIGMAFIVE_API spot_light {
            bool visible; // TODO check the odering on this
        };

        using spot_light_manager = basic_component_manager<spot_light>;
    }
}

SIGMAFIVE_COMPONENT(sigmafive::graphics::spot_light)

#endif //SIGMAFIVE_ENGINE_GRAPHICS_SPOT_LIGHT_HPP
