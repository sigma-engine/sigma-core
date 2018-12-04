#include <sigma/graphics/texture.hpp>

namespace sigma {
namespace graphics {
    texture::texture(std::weak_ptr<sigma::context> context, const resource::key_type& key)
        : resource::base_resource::base_resource(context, key)
        , size_ { 0, 0 }
        , format_ { texture_format::RGB8 }
        , minification_filter_ { texture_filter::LINEAR }
        , magnification_filter_ { texture_filter::LINEAR }
        , mipmap_filter_ { texture_filter::LINEAR }
    {
    }

    texture::texture(std::weak_ptr<sigma::context> context,
        const resource::key_type& key,
        glm::ivec2 size,
        texture_format format,
        texture_filter minification_filter,
        texture_filter magnification_filter,
        texture_filter mipmap_filter,
        bool store_mipmaps)
        : resource::base_resource::base_resource(context, key)
        , size_ { size }
        , format_ { format }
        , minification_filter_ { minification_filter }
        , magnification_filter_ { magnification_filter }
        , mipmap_filter_ { mipmap_filter }
    {
        auto pixel_size = size_of_pixel();
        if (store_mipmaps) {
            mipmap_offsets_.resize(total_mipmap_count());

            std::size_t total_pixels = 0;
            for (std::size_t i = 0; i < mipmap_offsets_.size(); ++i) {
                mipmap_offsets_[i] = pixel_size * total_pixels;
                total_pixels += std::max(size.x >> i, 1) * std::max(size_.y >> i, 1);
            }

            data_.resize(total_pixels * pixel_size);
        } else {
            mipmap_offsets_.resize(1);
            mipmap_offsets_[0] = 0;
            data_.resize(size_.x * size_.y * pixel_size);
        }
    }

    texture::texture(std::weak_ptr<sigma::context> context,
        const resource::key_type& key,
        const image_t<rgb8_pixel_t>& image,
        texture_filter minification_filter,
        texture_filter magnification_filter,
        texture_filter mipmap_filter,
        bool store_mipmaps)
        : texture(context, key, image.size,
              texture_format::RGB8,
              minification_filter,
              magnification_filter,
              mipmap_filter,
              store_mipmaps)
    {
        // TODO generate the mipmaps
        std::copy(image.pixels.begin(), image.pixels.end(), (rgb8_pixel_t*)data_.data());
        assert(store_mipmaps == false);
    }

    texture::texture(std::weak_ptr<sigma::context> context,
        const resource::key_type& key,
        const image_t<rgba8_pixel_t>& image,
        texture_filter minification_filter,
        texture_filter magnification_filter,
        texture_filter mipmap_filter,
        bool store_mipmaps)
        : texture(context, key, image.size,
              texture_format::RGBA8,
              minification_filter,
              magnification_filter,
              mipmap_filter,
              store_mipmaps)
    {
        // TODO generate the mipmaps
        std::copy(image.pixels.begin(), image.pixels.end(), (rgba8_pixel_t*)data_.data());
        assert(store_mipmaps == false);
    }

    texture::texture(std::weak_ptr<sigma::context> context,
        const resource::key_type& key,
        const image_t<rgb32f_pixel_t>& image,
        texture_filter minification_filter,
        texture_filter magnification_filter,
        texture_filter mipmap_filter,
        bool store_mipmaps)
        : texture(context, key, image.size,
              texture_format::RGB32F,
              minification_filter,
              magnification_filter,
              mipmap_filter,
              store_mipmaps)
    {
        // TODO generate the mipmaps
        std::copy(image.pixels.begin(), image.pixels.end(), (rgb32f_pixel_t*)data_.data());
        assert(store_mipmaps == false);
    }

    const glm::ivec2& texture::size() const noexcept
    {
        return size_;
    }

    texture_format texture::format() const noexcept
    {
        return format_;
    }

    texture_filter texture::minification_filter() const noexcept
    {
        return minification_filter_;
    }

    texture_filter texture::magnification_filter() const noexcept
    {
        return magnification_filter_;
    }

    texture_filter texture::mipmap_filter() const noexcept
    {
        return mipmap_filter_;
    }

    std::size_t texture::size_of_pixel() const noexcept
    {
        switch (format_) {
        case texture_format::RGB8:
            return sizeof(rgb8_pixel_t);
        case texture_format::RGBA8:
            return sizeof(rgba8_pixel_t);
        case texture_format::RGB32F:
            return sizeof(rgb32f_pixel_t);
        }
        return 0;
    }

    std::size_t texture::total_mipmap_count() const noexcept
    {
        return 1 + std::floor(std::log2(std::max(size_.x, size_.y)));
    }

    std::size_t texture::stored_mipmap_count() const noexcept
    {
        return mipmap_offsets_.size();
    }

    char* texture::data(std::size_t level)
    {
        assert(level < stored_mipmap_count());
        return data_.data() + mipmap_offsets_[level];
    }

    const char* texture::data(std::size_t level) const
    {
        assert(level < stored_mipmap_count());
        return data_.data() + mipmap_offsets_[level];
    }
}
}
