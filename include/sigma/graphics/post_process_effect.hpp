#ifndef SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
#define SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/texture.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/utility.hpp>

#include <string>
#include <unordered_map>

namespace sigma {
namespace graphics {
    struct post_process_effect_data {
        std::unordered_map<shader_type,resource::identifier> shaders{{shader_type::vertex,resource::identifier{"vertex://fullscreen_quad"}}};
        std::unordered_map<std::string,resource::identifier> textures;
        resource::identifier mesh = "static_mesh://fullscreen_quad:plane";

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& shaders;
            ar& textures;
            ar& mesh;
        }
    };

    class SIGMA_API post_process_effect {
    public:
        using resource_data = post_process_effect_data;

        post_process_effect() = default;

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

        virtual ~post_process_effect() = default;
    private:
        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;
    };


    using post_process_effect_manager = resource::manager<post_process_effect>;
}
}

#endif //SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
