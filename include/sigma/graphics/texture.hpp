#ifndef SIGMA_GRAPHICS_TEXTURE_HPP
#define SIGMA_GRAPHICS_TEXTURE_HPP

#include <sigma/config.hpp>
#include <sigma/reflect/reflect.hpp>
#include <sigma/resource/resource_cache.hpp>
#include <sigma/resource/identifier.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

namespace sigma {
namespace graphics {

    RCLASS()
    class SIGMA_API texture {
    public:
        texture() = default;

        texture(texture&&) noexcept = default;

        texture(const texture&) = delete;

        texture& operator=(texture&&) noexcept = default;

        texture& operator=(const texture&) = delete;

        ~texture() = default;

        unsigned int width() const;

        unsigned int height() const;

        const std::vector<unsigned char>& data() const;

        void set_data(unsigned int width, unsigned int height,const std::vector<unsigned char>& data);

    private:
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
        std::vector<unsigned char> data_;

		friend class resource::resource_cache<texture>;
		std::size_t reference_count = 0;
    };

	using texture_cache = resource::resource_cache<texture>;
}
}

#endif // SIGMA_GRAPHICS_TEXTURE_HPP
