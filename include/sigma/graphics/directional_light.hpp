#ifndef SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP
#define SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP

#include <sigma/config.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct SIGMA_API directional_light {
        glm::vec3 color;
        float intensity;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& color;
            ar& intensity;
        }
    };
}
}

#endif // SIGMA_GRAPHICS_DIRECTIONAL_LIGHT_HPP
