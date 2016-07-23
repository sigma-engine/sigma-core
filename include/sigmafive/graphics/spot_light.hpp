#ifndef SIGMAFIVE_ENGINE_GRAPHICS_SPOT_LIGHT_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_SPOT_LIGHT_HPP

#include <sigmafive/config.hpp>

#include <sigmafive/component.hpp>
#include <sigmafive/reflect/reflect.hpp>

namespace sigmafive {
namespace graphics {

    RCLASS()
    struct SIGMAFIVE_API spot_light {
    };

    using spot_light_manager = basic_component_manager<spot_light>;
}
}

SIGMAFIVE_COMPONENT(sigmafive::graphics::spot_light)

#endif // SIGMAFIVE_ENGINE_GRAPHICS_SPOT_LIGHT_HPP
