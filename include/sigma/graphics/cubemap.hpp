#ifndef SIGMA_GRAPHICS_CUBEMAP_HPP
#define SIGMA_GRAPHICS_CUBEMAP_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/texture.hpp>
#include <sigma/resource/cache.hpp>

#include <boost/serialization/version.hpp>

namespace sigma {
namespace graphics {

    struct cubemap {
        resource::handle<graphics::texture> right;
        resource::handle<graphics::texture> left;
        resource::handle<graphics::texture> top;
        resource::handle<graphics::texture> bottom;
        resource::handle<graphics::texture> back;
        resource::handle<graphics::texture> front;

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
