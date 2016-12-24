#ifndef SIGMA_GRAPHICS_POINT_LIGHT_HPP
#define SIGMA_GRAPHICS_POINT_LIGHT_HPP

#include <sigma/config.hpp>

#include <sigma/component.hpp>
#include <sigma/reflect/reflect.hpp>

namespace sigma {
namespace graphics {

    RCLASS()
    struct SIGMA_API point_light {
    };

    using point_light_manager = basic_component_manager<point_light>;
}
}

SIGMA_COMPONENT(sigma::graphics::point_light)

#endif // SIGMA_GRAPHICS_POINT_LIGHT_HPP
