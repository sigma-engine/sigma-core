// TEMPLATE(generated.hpp)
#ifndef SIGMA_GRAPHICS_SPOT_LIGHT_HPP
#define SIGMA_GRAPHICS_SPOT_LIGHT_HPP

#include <sigma/config.hpp>

#include <sigma/reflect.hpp>

#include <sigma/frustum.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct R_EXPORT() spot_light {
        BOOST_HANA_DEFINE_STRUCT(
            spot_light,
            (glm::vec3, color),
            (float, intensity),
            (float, cutoff),
            (bool, cast_shadows));

        glm::vec3 direction{ 0.0f, 1.0f, 0.0f };
        frustum shadow_frustum{ 2.0f * cutoff, 1.0f, .01f, 100.0f };

        spot_light()
            : color{ 1.0f, 1.0f, 1.0f }
            , intensity{ 1.0f }
            , cutoff{ 0.3926991f }
            , cast_shadows{ true }
        {
        }

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

#endif // SIGMA_GRAPHICS_SPOT_LIGHT_HPP
