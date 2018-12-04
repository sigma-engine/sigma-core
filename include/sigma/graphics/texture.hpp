#ifndef SIGMA_GRAPHICS_TEXTURE_HPP
#define SIGMA_GRAPHICS_TEXTURE_HPP

#include <sigma/config.hpp>
#include <sigma/resource/resource.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <cereal/types/vector.hpp>
#include <glm/vec2.hpp>

#include <vector>

namespace sigma {
namespace graphics {
    struct rgb8_pixel_t {
        uint8_t r, g, b;
    };

    struct rgba8_pixel_t {
        uint8_t r, g, b, a;
    };

    struct rgb32f_pixel_t {
        float r, g, b;
    };

    template <class T>
    struct image_t {
        using pixel_type = T;
        glm::ivec2 size;
        std::vector<pixel_type> pixels;
        image_t()
            : size({ 0, 0 })
        {
        }

        image_t(glm::ivec2 size)
            : size(size)
            , pixels(size.x * size.y)
        {
        }
    };

    template <class T>
    struct channel_count {
    };

    template <class T>
    struct channel_count<image_t<T>> : public channel_count<T> {
    };

    template <>
    struct channel_count<rgb8_pixel_t> {
        static constexpr const size_t value = 3;
    };

    template <>
    struct channel_count<rgba8_pixel_t> {
        static constexpr const size_t value = 4;
    };

    template <>
    struct channel_count<rgb32f_pixel_t> {
        static constexpr const size_t value = 3;
    };

    template <class T>
    inline constexpr size_t channel_count_v = channel_count<T>::value;

    enum class texture_filter {
        LINEAR,
        NEAREST,
        NONE
    };

    enum class texture_format {
        RGB8,
        RGBA8,
        RGB16F,
        RGBA16F,
        RGB32F,
        DEPTH32F_STENCIL8
    };

    enum class texture_sampler_type {
        SAMPLER2D,
        SAMPLER2D_ARRAY_SHADOW
    };

    struct texture_schema {
        texture_sampler_type type;
        size_t descriptor_set;
        size_t binding_point;
        std::string name;

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(type, descriptor_set, binding_point, name);
        }
    };

    template <class T>
    struct texture_type_for_pixel;

    template <>
    struct texture_type_for_pixel<rgb8_pixel_t> {
        static const constexpr texture_format value = texture_format::RGB8;
    };

    template <>
    struct texture_type_for_pixel<rgba8_pixel_t> {
        static const constexpr texture_format value = texture_format::RGBA8;
    };

    template <>
    struct texture_type_for_pixel<rgb32f_pixel_t> {
        static const constexpr texture_format value = texture_format::RGB32F;
    };

    class texture : public resource::base_resource {
    public:
        texture(std::weak_ptr<sigma::context> context, const resource::key_type& key);

        texture(std::weak_ptr<sigma::context> context,
            const resource::key_type& key,
            glm::ivec2 size,
            texture_format format,
            texture_filter minification_filter = texture_filter::LINEAR,
            texture_filter magnification_filter = texture_filter::LINEAR,
            texture_filter mipmap_filter = texture_filter::LINEAR,
            bool store_mipmaps = false);

        texture(std::weak_ptr<sigma::context> context,
            const resource::key_type& key,
            const image_t<rgb8_pixel_t>& image,
            texture_filter minification_filter = texture_filter::LINEAR,
            texture_filter magnification_filter = texture_filter::LINEAR,
            texture_filter mipmap_filter = texture_filter::LINEAR,
            bool store_mipmaps = false);

        texture(std::weak_ptr<sigma::context> context,
            const resource::key_type& key,
            const image_t<rgba8_pixel_t>& image,
            texture_filter minification_filter = texture_filter::LINEAR,
            texture_filter magnification_filter = texture_filter::LINEAR,
            texture_filter mipmap_filter = texture_filter::LINEAR,
            bool store_mipmaps = false);

        texture(std::weak_ptr<sigma::context> context,
            const resource::key_type& key,
            const image_t<rgb32f_pixel_t>& image,
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

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(
                size_,
                format_,
                minification_filter_,
                magnification_filter_,
                mipmap_filter_,
                mipmap_offsets_,
                data_);
        }

    private:
        glm::ivec2 size_;
        texture_format format_;
        texture_filter minification_filter_;
        texture_filter magnification_filter_;
        texture_filter mipmap_filter_;
        std::vector<std::size_t> mipmap_offsets_;
        std::vector<char> data_;
    };
}
}

REGISTER_RESOURCE(sigma::graphics::texture, texture, 1);

#endif // SIGMA_GRAPHICS_TEXTURE_HPP
