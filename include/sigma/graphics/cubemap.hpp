#ifndef SIGMA_GRAPHICS_CUBEMAP_HPP
#define SIGMA_GRAPHICS_CUBEMAP_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/texture.hpp>

#include <array>

namespace sigma {
namespace graphics {
    struct cubemap {
        enum class face : unsigned int {
            POSITIVE_X,
            NEGATIVE_X,
            POSITIVE_Y,
            NEGATIVE_Y,
            POSITIVE_Z,
            NEGATIVE_Z
        };

        std::array<std::shared_ptr<texture>, 6> faces;
    };
}
}

#endif // SIGMA_GRAPHICS_CUBEMAP_HPP
