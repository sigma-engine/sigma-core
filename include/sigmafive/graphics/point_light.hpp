#ifndef SIGMAFIVE_ENGINE_GRAPHICS_POINT_LIGHT_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_POINT_LIGHT_HPP

#include <sigmafive/config.hpp>

#include <sigmafive/component.hpp>
#include <sigmafive/reflect/reflect.hpp>

namespace sigmafive {
namespace graphics {

    RCLASS()
    struct SIGMAFIVE_API point_light {
    };

    using point_light_manager = basic_component_manager<point_light>;
}
}

SIGMAFIVE_COMPONENT(sigmafive::graphics::point_light)

#endif // SIGMAFIVE_ENGINE_GRAPHICS_POINT_LIGHT_HPP
