#ifndef SIGMAFIVE_ENGINE_GRAPHICS_POINT_LIGHT_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_POINT_LIGHT_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/component.hpp>

namespace sigmafive {
    namespace graphics {
        struct SIGMAFIVE_API point_light {
            bool visible; // TODO check the odering on this
        };

        using point_light_manager = basic_component_manager<point_light>;
    }
}

SIGMAFIVE_COMPONENT(sigmafive::graphics::point_light)

#endif //SIGMAFIVE_ENGINE_GRAPHICS_POINT_LIGHT_HPP
