#ifndef SIGMA_ENGINE_GRAPHICS_SHADER_TECHNIQUE_HPP
#define SIGMA_ENGINE_GRAPHICS_SHADER_TECHNIQUE_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/reflect/reflect.hpp>
#include <sigma/resource/identifier.hpp>
#include <sigma/resource/resource_cache.hpp>

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

        std::unordered_map<shader_type, resource::identifier> shaders;
        std::unordered_map<std::string, resource::identifier> textures;

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

        template <class>
        friend class resource::resource_cache;
        std::size_t reference_count = 0;
    };

    template <class T>
    class shader_technique_cache : public resource::resource_cache<T> {
    public:
        shader_technique_cache(boost::filesystem::path cache_directory, texture_cache& textures, shader_cache& shaders)
            : resource::resource_cache<T>(cache_directory)
            , textures_(textures)
            , shaders_(shaders)
        {
        }

        virtual bool increment_reference(resource::identifier resource_id) override
        {
            auto r = resource::resource_cache<T>::increment_reference(resource_id);
            if (r) {
                auto& tech = resources_[resource_id];

                // TODO shaders

                // TODO what if a shader is missing???
                for (const auto& shdr : tech.shaders)
                    shaders_.increment_reference(shdr.second);

                // TODO what texture is missing???
                for (const auto& txt : tech.textures)
                    textures_.increment_reference(txt.second);
            }
            return r;
        }

        virtual bool decrement_reference(resource::identifier resource_id) override
        {
            auto r = resource::resource_cache<T>::decrement_reference(resource_id);
            if (is_cached(resource_id)) {
                auto& tech = resources_[resource_id];
                // TODO shaders
                for (const auto& shdr : tech.shaders)
                    shaders_.decrement_reference(shdr.second);
                for (const auto& txt : tech.textures)
                    textures_.decrement_reference(txt.second);
            }
            return r;
        }

    private:
        texture_cache& textures_;
        shader_cache& shaders_;
    };
}
}

#endif // SIGMA_ENGINE_GRAPHICS_SHADER_TECHNIQUE_HPP
