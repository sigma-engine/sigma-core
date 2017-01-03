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

    class SIGMA_API post_process_effect {
    public:
        post_process_effect();

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

        static_mesh_cache::instance mesh() const;

        void set_mesh(static_mesh_cache::instance mesh);

        std::unordered_map<shader_type, shader_cache::instance> shaders;
        std::unordered_map<std::string, texture_cache::instance> textures;

    private:
        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;

        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& shaders;
            ar& textures;
            ar& mesh_;
        }

        static_mesh_cache::instance mesh_;
    };

    class post_process_effect_cache : public resource::cache<post_process_effect> {
    public:
        post_process_effect_cache(boost::filesystem::path cache_directory, texture_cache& textures, shader_cache& shaders, static_mesh_cache& static_meshes)
            : resource::cache<post_process_effect>(cache_directory)
            , textures_(textures)
            , shaders_(shaders)
            , static_meshes_(static_meshes)
        {
        }

        virtual void patch(std::shared_ptr<post_process_effect> effect) override
        {
            for (auto& tex : effect->textures)
                tex.second = textures_.get(tex.second);
            for (auto& shd : effect->shaders)
                shd.second = shaders_.get(shd.second);
            effect->set_mesh(static_meshes_.get(effect->mesh()));
        }

    private:
        texture_cache& textures_;
        shader_cache& shaders_;
        static_mesh_cache& static_meshes_;
    };
}
}

#endif //SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
