#ifndef SIGMA_GRAPHICS_TEXTURE_HPP
#define SIGMA_GRAPHICS_TEXTURE_HPP

#include <sigma/config.hpp>
#include <sigma/reflect.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/vec2.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

#include <vector>

namespace sigma {
namespace graphics {

    enum class R_EXPORT() texture_filter {
        LINEAR,
        NEAREST,
        NONE
    };

    enum class R_EXPORT() texture_format {
        RGB8,
        RGBA8,
        RGB32F
    };

    struct texture {
        BOOST_HANA_DEFINE_STRUCT(
            texture,
            (glm::ivec2, size),
            (texture_format, format),
            (std::vector<char>, data),
            (texture_filter, minification_filter),
            (texture_filter, magnification_filter),
            (texture_filter, mipmap_filter));

        texture()
            : minification_filter{ texture_filter::LINEAR }
            , magnification_filter{ texture_filter::LINEAR }
            , mipmap_filter{ texture_filter::LINEAR }
        {
        }

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& size;
            ar& format;
            ar& data;
            ar& minification_filter;
            ar& magnification_filter;
            ar& mipmap_filter;
        }
    };
}
}

BOOST_CLASS_VERSION(sigma::graphics::texture, 1);

#endif // SIGMA_GRAPHICS_TEXTURE_HPP
