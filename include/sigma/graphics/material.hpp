#ifndef SIGMA_ENGINE_GRAPHICS_MATERIAL_HPP
#define SIGMA_ENGINE_GRAPHICS_MATERIAL_HPP

#include <sigma/config.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

#include <sigma/reflect/reflect.hpp>
#include <sigma/resource/identifier.hpp>

namespace sigma {
namespace graphics {

    RCLASS()
    class sigma_API material {
    public:
        material() = default;

        material(const material&) = delete;

        material(material&&) = default;

        material& operator=(const material&) = delete;

        material& operator=(material&&) = default;

        ~material() = default;

        resource::identifier vertex_shader;
        resource::identifier fragment_shader;
        std::vector<std::pair<std::string, resource::identifier> > textures;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& vertex_shader;
            ar& fragment_shader;
            ar& textures;
        }
    };

    class sigma_API material_cache {
    public:
        material_cache() = default;

        material_cache(material_cache&&) noexcept = default;

        material_cache(const material_cache&) = delete;

        material_cache& operator=(material_cache&&) noexcept = default;

        material_cache& operator=(const material_cache&) = delete;

        virtual ~material_cache() = default;

        /**
        * @brief Returns if material is loaded in this cache.
        *
        * @param material the material to check if cached.
        * @return true if the material is cache.
        */
        virtual bool is_cached(resource::identifier material) const = 0;

        /**
        * @brief Increases the reference count associated with the material.
        *
        *
        * @param material the material to increase the reference count of.
        * @return true if the material exists and is valid.
        */
        virtual bool increment_reference(resource::identifier material) = 0;

        /**
        * @brief Decreases the reference count associated with
        * the material.
        *
        *
        * @param material the material to decrease the reference count of.
        * @returns true if the material reference count is zero.
        */
        virtual bool decrement_reference(resource::identifier material) = 0;
    };
}
}

#endif // SIGMA_ENGINE_GRAPHICS_MATERIAL_HPP
