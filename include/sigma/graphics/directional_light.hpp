#ifndef SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP
#define SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP

#include <sigma/config.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct SIGMA_API directional_light {
        glm::vec3 color;
        float intensity;
        bool cast_shadows = true;
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

#endif // SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP
