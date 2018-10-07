#include <sigma/graphics/texture.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/gil/image.hpp>

#include <iostream>

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
        const boost::gil::rgb8c_view_t& view,
        texture_filter minification_filter,
        texture_filter magnification_filter,
        texture_filter mipmap_filter,
        bool store_mipmaps)
        : texture(context, key, { view.width(), view.height() },
              texture_format::RGB8,
              minification_filter,
              magnification_filter,
              mipmap_filter,
              store_mipmaps)
    {
        boost::gil::copy_pixels(view, boost::gil::interleaved_view(size_.x, size_.y, (boost::gil::rgb8_pixel_t*)data_.data(), size_.x * sizeof(boost::gil::rgb8_pixel_t)));
        // TODO generate the mipmaps
        assert(store_mipmaps == false);
    }

    texture::texture(std::weak_ptr<sigma::context> context,
        const resource::key_type& key,
        const boost::gil::rgba8c_view_t& view,
        texture_filter minification_filter,
        texture_filter magnification_filter,
        texture_filter mipmap_filter,
        bool store_mipmaps)
        : texture(context, key, { view.width(), view.height() },
              texture_format::RGBA8,
              minification_filter,
              magnification_filter,
              mipmap_filter,
              store_mipmaps)
    {
        boost::gil::copy_pixels(view, boost::gil::interleaved_view(size_.x, size_.y, (boost::gil::rgba8_pixel_t*)data_.data(), size_.x * sizeof(boost::gil::rgba8_pixel_t)));
        // TODO generate the mipmaps
        assert(store_mipmaps == false);
    }

    texture::texture(std::weak_ptr<sigma::context> context,
        const resource::key_type& key,
        const boost::gil::rgb32fc_view_t& view,
        texture_filter minification_filter,
        texture_filter magnification_filter,
        texture_filter mipmap_filter,
        bool store_mipmaps)
        : texture(context, key, { view.width(), view.height() },
              texture_format::RGB32F,
              minification_filter,
              magnification_filter,
              mipmap_filter,
              store_mipmaps)
    {
        boost::gil::copy_pixels(view, boost::gil::interleaved_view(size_.x, size_.y, (boost::gil::rgb32f_pixel_t*)data_.data(), size_.x * sizeof(boost::gil::rgb32f_pixel_t)));
        // TODO generate the mipmaps
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
            return sizeof(boost::gil::rgb8_pixel_t);
        case texture_format::RGBA8:
            return sizeof(boost::gil::rgba8_pixel_t);
        case texture_format::RGB32F:
            return sizeof(boost::gil::rgb32f_pixel_t);
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

    void to_json(nlohmann::json& j, const texture_filter& flt)
    {
        static std::map<texture_filter, std::string> filter_map = {
            { texture_filter::NEAREST, "NEAREST" },
            { texture_filter::LINEAR, "LINEAR" },
            { texture_filter::NONE, "NONE" }
        };

        j = filter_map.at(flt);
    }

    void from_json(const nlohmann::json& j, texture_filter& flt)
    {
        static std::map<std::string, texture_filter> filter_map = {
            { "NEAREST", texture_filter::NEAREST },
            { "LINEAR", texture_filter::LINEAR },
            { "NONE", texture_filter::NONE }
        };

        auto str_val = boost::to_upper_copy(j.get<std::string>());
        auto it = filter_map.find(str_val);

        if (it != filter_map.end())
            flt = it->second;
        else
            flt = texture_filter::LINEAR;
    }

    void to_json(nlohmann::json& j, const texture_format& fmt)
    {
        static std::map<texture_format, std::string> format_map = {
            { texture_format::RGB8, "RGB8" },
            { texture_format::RGBA8, "RGBA8" },
            { texture_format::RGB16F, "RGB16F" },
            { texture_format::RGBA16F, "RGBA16F" },
            { texture_format::RGB32F, "RGB32F" },
            { texture_format::DEPTH32F_STENCIL8, "DEPTH32F_STENCIL8" }
        };

        j = format_map.at(fmt);
    }

    void from_json(const nlohmann::json& j, texture_format& fmt)
    {
        static std::map<std::string, texture_format> format_map = {
            { "RGB8", texture_format::RGB8 },
            { "RGBA8", texture_format::RGBA8 },
            { "RGB16F", texture_format::RGB16F },
            { "RGBA16F", texture_format::RGBA16F },
            { "RGB32F", texture_format::RGB32F },
            { "DEPTH32F_STENCIL8", texture_format::DEPTH32F_STENCIL8 }
        };

        auto str_val = boost::to_upper_copy(j.get<std::string>());
        auto it = format_map.find(str_val);

        if (it != format_map.end())
            fmt = it->second;
        else
            fmt = texture_format::RGB8;
    }
}
}
