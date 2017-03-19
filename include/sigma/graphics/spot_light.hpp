#ifndef SIGMA_GRAPHICS_SPOT_LIGHT_HPP
#define SIGMA_GRAPHICS_SPOT_LIGHT_HPP

#include <sigma/config.hpp>

namespace sigma {
namespace graphics {

    struct SIGMA_API spot_light {
        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
        }
    };
}
}

#endif // SIGMA_GRAPHICS_SPOT_LIGHT_HPP
