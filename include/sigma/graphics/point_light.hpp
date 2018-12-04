#ifndef SIGMA_GRAPHICS_POINT_LIGHT_HPP
#define SIGMA_GRAPHICS_POINT_LIGHT_HPP

#include <sigma/config.hpp>

#include <sigma/component.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct point_light {
        glm::vec3 color;
        float intensity;

        point_light(const glm::vec3& color = { 1.0f, 1.0f, 1.0f }, float intensity = 1.0f)
            : color { color }
            , intensity { intensity }
        {
        }

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar(color, intensity);
        }
    };
}
}

REGISTER_COMPONENT(sigma::graphics::point_light)

#endif // SIGMA_GRAPHICS_POINT_LIGHT_HPP
