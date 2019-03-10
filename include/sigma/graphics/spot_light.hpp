#ifndef SIGMA_GRAPHICS_SPOT_LIGHT_HPP
#define SIGMA_GRAPHICS_SPOT_LIGHT_HPP

#include <sigma/config.hpp>

#include <sigma/frustum.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct spot_light {
        glm::vec3 color;
        float intensity;
        float cutoff;
        bool cast_shadows;

        glm::vec3 direction;
        frustum shadow_frustum;

        spot_light(const glm::vec3& color = { 1.0f, 1.0f, 1.0f },
            float intensity = 1.0f,
            float cutoff = 0.3926991f,
            bool cast_shadows = true,
            const glm::vec3& direction = { 0.0f, 1.0f, 0.0f },
            const frustum& shadow_frustum = { 2.0f * 0.3926991f, 1.0f, .01f, 100.0f })
            : color { color }
            , intensity { intensity }
            , cutoff { cutoff }
            , cast_shadows { cast_shadows }
            , direction { direction }
            , shadow_frustum { shadow_frustum }
        {
        }

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar(color, intensity, cutoff, cast_shadows);
        }
    };
}
}

#endif // SIGMA_GRAPHICS_SPOT_LIGHT_HPP
