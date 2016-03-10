#ifndef SIGMAFIVE_ENGINE_GRAPHICS_MATERIAL_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_MATERIAL_HPP

#include <sigmafive/config.hpp>

#include <array>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <sigmafive/resource/identifier.hpp>
#include <vector>

namespace sigmafive {
namespace graphics {
    class SIGMAFIVE_API material {
    public:
        material() = default;

        material(material&&) = default;

        material& operator=(const material&) = delete;

        material& operator=(material&&) = default;

        const std::vector<resource::identifier>& get_shaders() const;

        void add_shader(resource::identifier shader);

        void remove_shader(resource::identifier shader);

    private:
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& shaders_;
        }
        std::vector<resource::identifier> shaders_;
    };

    class material_cache {
    public:
        material_cache() = default;

        material_cache(const material_cache&) = delete;

        virtual ~material_cache() = default;

        material_cache& operator=(const material_cache&) = delete;

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
        * NOTE calling this method will not invalidate any references to other materiales
        * in this cache.
        *
        * @param material the material to increase the reference count of.
        * @return true if the material exists and is valid.
        */
        virtual bool increment_reference(resource::identifier material) = 0;

        /**
        * @brief Decreases the reference count associated with
        * the material.
        *
        * NOTE calling this method wiil not invalidate any references to other materiales
        * in this cache.
        *
        * @param material the material to decrease the reference count of.
        * @returns true if the material reference count is zero.
        */
        virtual bool decrement_reference(resource::identifier material) = 0;

        //virtual material& get(resource::identifier material) = 0;

        //virtual const material& add(resource::identifier material) const = 0;
    };
}
}

#endif //SIGMAFIVE_ENGINE_GRAPHICS_MATERIAL_HPP
