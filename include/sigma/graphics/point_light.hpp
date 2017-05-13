#ifndef SIGMA_GRAPHICS_POINT_LIGHT_HPP
#define SIGMA_GRAPHICS_POINT_LIGHT_HPP

#include <sigma/config.hpp>
#include <sigma/reflect.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct R_COMPONENT() point_light {
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

#include <sigma/graphics/point_light.generated.hpp>

#endif // SIGMA_GRAPHICS_POINT_LIGHT_HPP
