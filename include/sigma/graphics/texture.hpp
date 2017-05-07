#ifndef SIGMA_GRAPHICS_TEXTURE_HPP
#define SIGMA_GRAPHICS_TEXTURE_HPP

#include <sigma/config.hpp>
#include <sigma/resource/manager.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/vec2.hpp>

#include <boost/serialization/vector.hpp>

namespace sigma {
namespace graphics {

    enum class texture_filter {
        LINEAR,
        NEAREST,
        NONE
    };

    enum class texture_format {
        RGB8,
        RGBA8,
        RGB32F
    };

    struct texture_data {
        glm::ivec2 size;
        texture_format format;
        std::vector<char> data;
        texture_filter minification_filter = texture_filter::LINEAR;
        texture_filter magnification_filter = texture_filter::LINEAR;
        texture_filter mipmap_filter = texture_filter::LINEAR;

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

    class SIGMA_API texture {
    public:
        using resource_data = texture_data;

        texture() = default;

        texture(texture&&) noexcept = default;

        texture& operator=(texture&&) noexcept = default;

        virtual ~texture() = default;

    private:
        texture(const texture&) = delete;
        texture& operator=(const texture&) = delete;
    };

    using texture_manager = resource::manager<texture>;
}
}

BOOST_CLASS_VERSION(sigma::graphics::texture_data, 1);

#endif // SIGMA_GRAPHICS_TEXTURE_HPP
