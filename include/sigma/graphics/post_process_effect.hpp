#ifndef SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
#define SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/resource/resource.hpp>

namespace sigma {
namespace graphics {
    struct post_process_effect : public technique_uniform_data {
        resource::handle<technique> technique_id;
        resource::handle<static_mesh> mesh;

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::base_class<technique_uniform_data>(this), technique_id,mesh);
        }
    };
}
}

REGISTER_RESOURCE(sigma::graphics::post_process_effect, post_process_effect, 1)

#endif //SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
