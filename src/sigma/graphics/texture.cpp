#include <sigma/graphics/texture.hpp>

namespace sigma {
namespace graphics {
    texture::texture()
        : size_{ 0, 0 }
        , format_{ texture_format::RGB8 }
        , minification_filter_{ texture_filter::LINEAR }
        , magnification_filter_{ texture_filter::LINEAR }
        , mipmap_filter_{ texture_filter::LINEAR }
    {
    }

    texture::texture(const boost::gil::rgb8c_view_t& view, texture_filter minification_filter, texture_filter magnification_filter, texture_filter mipmap_filter)
        : size_{ view.width(), view.height() }
        , format_{ texture_format::RGB8 }
        , minification_filter_{ minification_filter }
        , magnification_filter_{ magnification_filter }
        , mipmap_filter_{ mipmap_filter }
    {
        data_.resize(view.width() * view.height() * sizeof(boost::gil::rgb8_pixel_t));
        boost::gil::copy_pixels(view, boost::gil::interleaved_view(size_.x, size_.y, (boost::gil::rgb8_pixel_t*)data_.data(), size_.x * sizeof(boost::gil::rgb8_pixel_t)));
    }

    texture::texture(const boost::gil::rgba8c_view_t& view, texture_filter minification_filter, texture_filter magnification_filter, texture_filter mipmap_filter)
        : size_{ view.width(), view.height() }
        , format_{ texture_format::RGBA8 }
        , minification_filter_{ minification_filter }
        , magnification_filter_{ magnification_filter }
        , mipmap_filter_{ mipmap_filter }
    {
        data_.resize(view.width() * view.height() * sizeof(boost::gil::rgba8_pixel_t));
        boost::gil::copy_pixels(view, boost::gil::interleaved_view(size_.x, size_.y, (boost::gil::rgba8_pixel_t*)data_.data(), size_.x * sizeof(boost::gil::rgba8_pixel_t)));
    }

    texture::texture(const boost::gil::rgb32fc_view_t& view, texture_filter minification_filter, texture_filter magnification_filter, texture_filter mipmap_filter)
        : size_{ view.width(), view.height() }
        , format_{ texture_format::RGB32F }
        , minification_filter_{ minification_filter }
        , magnification_filter_{ magnification_filter }
        , mipmap_filter_{ mipmap_filter }
    {
        data_.resize(view.width() * view.height() * sizeof(boost::gil::rgb32f_pixel_t));
        boost::gil::copy_pixels(view, boost::gil::interleaved_view(size_.x, size_.y, (boost::gil::rgb32f_pixel_t*)data_.data(), size_.x * sizeof(boost::gil::rgb32f_pixel_t)));
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

    const char* texture::data() const
    {
        return data_.data();
    }
}
}
