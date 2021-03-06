#ifndef SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP
#define SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP

#include <sigma/config.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct directional_light {
        glm::vec3 color;
        float intensity;
        bool cast_shadows;
        glm::vec3 direction { 0.0f, 1.0f, 0.0f };

        directional_light(const glm::vec3& color = { 1.0f, 1.0f, 1.0f }, float intensity = 1.0f, bool cast_shadows = true, const glm::vec3& direction = { 0.0f, 1.0f, 0.0f })
            : color { color }
            , intensity { intensity }
            , cast_shadows { cast_shadows }
            , direction { direction }
        {
        }

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar(color, intensity, cast_shadows);
        }
    };
}
}

#endif // SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP
