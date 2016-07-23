#ifndef SIGMAFIVE_ENGINE_GRAPHICS_TEXTURE_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_TEXTURE_HPP

#include <sigmafive/config.hpp>

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include <sigmafive/reflect/reflect.hpp>
#include <sigmafive/resource/identifier.hpp>

namespace sigmafive {
namespace graphics {

    RCLASS()
    class SIGMAFIVE_API texture {
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

        void set_data(unsigned int width, unsigned int height,
            const std::vector<unsigned char>& data);

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
    };

    class SIGMAFIVE_API texture_cache {
    public:
        texture_cache() = default;

        texture_cache(texture_cache&&) noexcept = default;

        texture_cache(const texture_cache&) = delete;

        texture_cache& operator=(texture_cache&&) noexcept = default;

        texture_cache& operator=(const texture_cache&) = delete;

        virtual ~texture_cache() = default;

        /**
        * @brief Returns if texture is loaded in this cache.
        *
        * @param texture the texture to check if cached.
        * @return true if the texture is cache.
        */
        virtual bool is_cached(resource::identifier texture) const = 0;

        /**
        * @brief Increases the reference count associated with the texture.
        *
        *
        * @param texture the texture to increase the reference count of.
        * @return true if the texture exists and is valid.
        */
        virtual bool increment_reference(resource::identifier texture) = 0;

        /**
        * @brief Decreases the reference count associated with
        * the texture.
        *
        *
        * @param texture the texture to decrease the reference count of.
        * @returns true if the texture reference count is zero.
        */
        virtual bool decrement_reference(resource::identifier texture) = 0;
    };
}
}

#endif // SIGMAFIVE_ENGINE_GRAPHICS_TEXTURE_HPP
