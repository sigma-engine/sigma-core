#ifndef SIGMA_GRAPHICS_CUBEMAP_HPP
#define SIGMA_GRAPHICS_CUBEMAP_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/cache.hpp>
#include <sigma/resource/resource.hpp>

#include <boost/serialization/array.hpp>

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

        std::array<resource::handle<graphics::texture>, 6> faces;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& faces;
        }
    };
}
}

REGISTER_RESOURCE(sigma::graphics::cubemap, cubemap, 1)

#endif // SIGMA_GRAPHICS_CUBEMAP_HPP
