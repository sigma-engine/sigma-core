#ifndef SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP
#define SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP

#include <sigma/config.hpp>
#include <sigma/reflect.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct R_COMPONENT() directional_light {
        glm::vec3 color;
        float intensity;
        bool cast_shadows = true;
        R_FIELD(derived)
        glm::vec3 direction{ 0.0f, 1.0f, 0.0f };

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& color;
            ar& intensity;
            ar& cast_shadows;
        }
    };
}
}

#include <sigma/graphics/directional_light.generated.hpp>

#endif // SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP
