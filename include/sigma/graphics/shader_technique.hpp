#ifndef SIGMA_ENGINE_GRAPHICS_SHADER_TECHNIQUE_HPP
#define SIGMA_ENGINE_GRAPHICS_SHADER_TECHNIQUE_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/reflect/reflect.hpp>
#include <sigma/resource/identifier.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/utility.hpp>

#include <unordered_map>

namespace sigma {
namespace graphics {
    class SIGMA_API shader_technique {
    public:
        shader_technique() = default;

        ~shader_technique() = default;

        shader_technique(shader_technique&&) = default;

        shader_technique& operator=(shader_technique&&) = default;

        std::unordered_map<shader_type, shader_cache::instance> shaders;
        std::unordered_map<std::string, texture_cache::instance> textures;

    private:
        shader_technique(const shader_technique&) = delete;
        shader_technique& operator=(const shader_technique&) = delete;

        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& shaders;
            ar& textures;
        }

    };

    template<class T>
    class shader_technique_cache : public resource::cache<T> {
    public:
        shader_technique_cache(boost::filesystem::path cache_directory,texture_cache &textures,shader_cache &shaders)
            : resource::cache<T>(cache_directory)
            , textures_(textures)
            , shaders_(shaders)
        {
        }

        virtual void patch(std::shared_ptr<T> tech) override
        {
            for(auto &tex:tech->textures)
                tex.second = textures_.get(tex.second.id());
            for(auto &shd:tech->shaders)
                shd.second = shaders_.get(shd.second.id());
        }
    private:
        texture_cache &textures_;
        shader_cache &shaders_;
    };
}
}

#endif // SIGMA_ENGINE_GRAPHICS_SHADER_TECHNIQUE_HPP
