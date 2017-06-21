#ifndef SIGMA_GRAPHICS_CUBEMAP_HPP
#define SIGMA_GRAPHICS_CUBEMAP_HPP

#include <boost/filesystem/path.hpp>
#include <boost/serialization/version.hpp>

namespace sigma {
namespace graphics {

    struct cubemap {
        boost::filesystem::path right;
        boost::filesystem::path left;
        boost::filesystem::path top;
        boost::filesystem::path bottom;
        boost::filesystem::path back;
        boost::filesystem::path front;

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
