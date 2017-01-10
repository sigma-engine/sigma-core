#ifndef SIGMA_GRAPHICS_CUBEMAP_HPP
#define SIGMA_GRAPHICS_CUBEMAP_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/texture.hpp>

namespace sigma {
namespace graphics {

    struct cubemap_data {
        texture_data right;
        texture_data left;
        texture_data top;
        texture_data bottom;
        texture_data back;
        texture_data front;
        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& right;
            ar& left;
            ar& top;
            ar& bottom;
            ar& back;
            ar& front;
        }
    };

    class SIGMA_API cubemap {
    public:
        using resource_data = cubemap_data;

        cubemap() = default;

        cubemap(cubemap&&) noexcept = default;

        cubemap& operator=(cubemap&&) noexcept = default;

        virtual ~cubemap() = default;

    private:
        cubemap(const cubemap&) = delete;
        cubemap& operator=(const cubemap&) = delete;
    };

    using cubemap_manager = resource::manager<cubemap>;
}
}

BOOST_CLASS_VERSION(sigma::graphics::cubemap_data, 1);

#endif // SIGMA_GRAPHICS_CUBEMAP_HPP
