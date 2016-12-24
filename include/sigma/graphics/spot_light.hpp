#ifndef SIGMA_GRAPHICS_SPOT_LIGHT_HPP
#define SIGMA_GRAPHICS_SPOT_LIGHT_HPP

#include <sigma/config.hpp>

#include <sigma/component.hpp>
#include <sigma/reflect/reflect.hpp>

namespace sigma {
namespace graphics {

    RCLASS()
    struct SIGMA_API spot_light {
    };

    using spot_light_manager = basic_component_manager<spot_light>;
}
}

#endif // SIGMA_GRAPHICS_SPOT_LIGHT_HPP
