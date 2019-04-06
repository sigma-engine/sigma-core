#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/resource/resource.hpp>

namespace sigma {
namespace graphics {
    struct material : public technique_uniform_data {
        resource::handle<technique> technique_id;
        bool transparent = false;

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::base_class<technique_uniform_data>(this), technique_id, transparent);
        }
    };
}
}

REGISTER_RESOURCE(sigma::graphics::material, material, 1)

#endif // SIGMA_GRAPHICS_MATERIAL_HPP
