#ifndef SIGMA_ENGINE_GRAPHICS_POINT_LIGHT_HPP
#define SIGMA_ENGINE_GRAPHICS_POINT_LIGHT_HPP

#include <sigma/config.hpp>

#include <sigma/component.hpp>
#include <sigma/reflect/reflect.hpp>

namespace sigma {
namespace graphics {

    RCLASS()
    struct sigma_API point_light {
    };

    using point_light_manager = basic_component_manager<point_light>;
}
}

sigma_COMPONENT(sigma::graphics::point_light)

#endif // SIGMA_ENGINE_GRAPHICS_POINT_LIGHT_HPP
