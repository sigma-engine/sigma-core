#ifndef SIGMA_GRAPHICS_SPOT_LIGHT_HPP
#define SIGMA_GRAPHICS_SPOT_LIGHT_HPP

#include <sigma/component.hpp>
#include <sigma/config.hpp>

namespace sigma {
namespace graphics {

    struct SIGMA_API spot_light {
    };

    using spot_light_manager = basic_component_manager<spot_light>;
}
}

#endif // SIGMA_GRAPHICS_SPOT_LIGHT_HPP
