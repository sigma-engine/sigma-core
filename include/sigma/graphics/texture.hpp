#ifndef SIGMA_GRAPHICS_TEXTURE_HPP
#define SIGMA_GRAPHICS_TEXTURE_HPP

#include <sigma/config.hpp>
#include <sigma/reflect.hpp>
#include <sigma/resource/resource.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/vec2.hpp>

#include <boost/gil/typedefs.hpp>

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

    template <class T>
    struct texture_type_for_pixel;

    template <>
    struct texture_type_for_pixel<boost::gil::rgb8_pixel_t> {
        static const constexpr texture_format value = texture_format::RGB8;
    };

    template <>
    struct texture_type_for_pixel<boost::gil::rgba8_pixel_t> {
        static const constexpr texture_format value = texture_format::RGBA8;
    };

    template <>
    struct texture_type_for_pixel<boost::gil::rgb32f_pixel_t> {
        static const constexpr texture_format value = texture_format::RGB32F;
    };

    class texture {
        glm::ivec2 size_;
        texture_format format_;
        texture_filter minification_filter_;
        texture_filter magnification_filter_;
        texture_filter mipmap_filter_;
        std::vector<std::size_t> mipmap_offsets_;
        std::vector<char> data_;

    public:
        texture();

        texture(glm::ivec2 size,
            texture_format format,
            texture_filter minification_filter = texture_filter::LINEAR,
            texture_filter magnification_filter = texture_filter::LINEAR,
            texture_filter mipmap_filter = texture_filter::LINEAR,
            bool store_mipmaps = false);

        texture(const boost::gil::rgb8c_view_t& view,
            texture_filter minification_filter = texture_filter::LINEAR,
            texture_filter magnification_filter = texture_filter::LINEAR,
            texture_filter mipmap_filter = texture_filter::LINEAR,
            bool store_mipmaps = false);

        texture(const boost::gil::rgba8c_view_t& view,
            texture_filter minification_filter = texture_filter::LINEAR,
            texture_filter magnification_filter = texture_filter::LINEAR,
            texture_filter mipmap_filter = texture_filter::LINEAR,
            bool store_mipmaps = false);

        texture(const boost::gil::rgb32fc_view_t& view,
            texture_filter minification_filter = texture_filter::LINEAR,
            texture_filter magnification_filter = texture_filter::LINEAR,
            texture_filter mipmap_filter = texture_filter::LINEAR,
            bool store_mipmaps = false);

        const glm::ivec2& size() const noexcept;

        texture_format format() const noexcept;

        texture_filter minification_filter() const noexcept;

        texture_filter magnification_filter() const noexcept;

        texture_filter mipmap_filter() const noexcept;

        std::size_t size_of_pixel() const noexcept;

        std::size_t total_mipmap_count() const noexcept;

        std::size_t stored_mipmap_count() const noexcept;

        char* data(std::size_t level);

        const char* data(std::size_t level) const;

        template <class PixelType>
        typename boost::gil::type_from_x_iterator<PixelType*>::view_t as_view(std::size_t level)
        {
            assert(texture_type_for_pixel<PixelType>::value == format_);
            assert(level < stored_mipmap_count());

            long int size_x = std::max(1, size_.x >> level);
            long int size_y = std::max(1, size_.y >> level);

            using view_type = typename boost::gil::type_from_x_iterator<PixelType*>::view_t;
            return view_type({ size_x, size_y }, typename view_type::locator((PixelType*)data(level), size_x * sizeof(PixelType)));
        }

        template <class PixelType>
        typename boost::gil::type_from_x_iterator<PixelType*>::cview_t as_view(std::size_t level) const
        {
            assert(texture_type_for_pixel<PixelType>::value == format_);
            assert(level < stored_mipmap_count());

            std::size_t size_x = std::max(1, size_.x >> level);
            std::size_t size_y = std::max(1, size_.y >> level);

            using view_type = typename boost::gil::type_from_x_iterator<PixelType*>::cview_t;
            return view_type({ size_x, size_y }, typename view_type::locator((PixelType*)data(level), size_x * sizeof(PixelType)));
        }

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(size_,
            format_,
            minification_filter_,
            magnification_filter_,
            mipmap_filter_,
            mipmap_offsets_,
            data_);
        }
    };
}
}

REGISTER_RESOURCE(sigma::graphics::texture, texture, 1)

#endif // SIGMA_GRAPHICS_TEXTURE_HPP
