#ifndef SIGMA_GRAPHICS_CUBEMAP_HPP
#define SIGMA_GRAPHICS_CUBEMAP_HPP

#include <sigma/graphics/texture.hpp>

#include <boost/serialization/version.hpp>

namespace sigma {
namespace graphics {

    struct cubemap {
        texture right;
        texture left;
        texture top;
        texture bottom;
        texture back;
        texture front;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& right;
            ar& left;
            ar& top;
            ar& bottom;
            ar& back;
            ar& front;
        }
    };
}
}

BOOST_CLASS_VERSION(sigma::graphics::cubemap, 1);

#endif // SIGMA_GRAPHICS_CUBEMAP_HPP
