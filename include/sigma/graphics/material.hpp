#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/cache.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/utility.hpp>

#include <string>
#include <unordered_map>

namespace sigma {
namespace graphics {

    RCLASS()
    class SIGMA_API material {
    public:
        material() = default;

        material(material&&) = default;

        material& operator=(material&&) = default;

        std::unordered_map<shader_type, shader_cache::instance> shaders;
        std::unordered_map<std::string, texture_cache::instance> textures;

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;

        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& shaders;
            ar& textures;
        }
    };

    class material_cache : public resource::cache<material> {
    public:
        material_cache(boost::filesystem::path cache_directory, texture_cache& textures, shader_cache& shaders)
            : resource::cache<material>(cache_directory)
            , textures_(textures)
            , shaders_(shaders)
        {
        }

        virtual void patch(std::shared_ptr<material> mat) override
        {
            for (auto& tex : mat->textures)
                tex.second = textures_.get(tex.second);
            for (auto& shd : mat->shaders)
                shd.second = shaders_.get(shd.second);
        }

    private:
        texture_cache& textures_;
        shader_cache& shaders_;
    };
}
}

#endif // SIGMA_GRAPHICS_MATERIAL_HPP
