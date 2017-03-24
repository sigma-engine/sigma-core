#ifndef SIGMA_GRAPHICS_SPOT_LIGHT_HPP
#define SIGMA_GRAPHICS_SPOT_LIGHT_HPP

#include <sigma/config.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {

    struct SIGMA_API spot_light {
        glm::vec3 color;
        float intensity;
        float cutoff;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& color;
            ar& intensity;
            ar& cutoff;
        }
    };
}
}

#endif // SIGMA_GRAPHICS_SPOT_LIGHT_HPP
