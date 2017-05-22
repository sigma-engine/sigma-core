// TEMPLATE(generated.hpp)
// TEMPLATE(generated.cpp)
#ifndef SIGMA_GRAPHICS_SPOT_LIGHT_HPP
#define SIGMA_GRAPHICS_SPOT_LIGHT_HPP

#include <sigma/config.hpp>
#include <sigma/reflect.hpp>

#include <sigma/frustum.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct R_EXPORT() spot_light {
        glm::vec3 color{ 1.0f, 1.0f, 1.0f };
        float intensity = 1.0f;
        float cutoff = 0.3926991f;
        bool cast_shadows = true;

        R_FIELD(derived)
        glm::vec3 direction{ 0.0f, 1.0f, 0.0f };

        R_FIELD(derived)
        frustum shadow_frustum{ 2.0f * cutoff, 1.0f, .01f, 100.0f };

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& color;
            ar& intensity;
            ar& cutoff;
            ar& cast_shadows;
        }
    };
}
}

#include <sigma/graphics/spot_light.generated.hpp>

#endif // SIGMA_GRAPHICS_SPOT_LIGHT_HPP
