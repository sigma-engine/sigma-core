#ifndef SIGMA_GRAPHICS_TEXTURE_HPP
#define SIGMA_GRAPHICS_TEXTURE_HPP

#include <sigma/config.hpp>
#include <sigma/reflect/reflect.hpp>
#include <sigma/resource/cache.hpp>

#include <glm/vec2.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

namespace sigma {
namespace graphics {

    RCLASS()
    class SIGMA_API texture {
    public:
        texture() = default;

        texture(texture&&) noexcept = default;

        texture& operator=(texture&&) noexcept = default;

        ~texture() = default;

        unsigned int width() const;

        unsigned int height() const;

        glm::ivec2 size() const;

        const std::vector<unsigned char>& data() const;

        void set_data(unsigned int width, unsigned int height, const std::vector<unsigned char>& data);

    private:
        texture(const texture&) = delete;
        texture& operator=(const texture&) = delete;

        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& width_;
            ar& height_;
            ar& data_;
        }

        unsigned int width_;
        unsigned int height_;
        // TODO use boost::gil
        std::vector<unsigned char> data_;
    };

    using texture_cache = resource::cache<texture>;
}
}

#endif // SIGMA_GRAPHICS_TEXTURE_HPP
