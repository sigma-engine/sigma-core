#ifndef SIGMAFIVE_ENGINE_GRAPHICS_SHADER_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_SHADER_HPP

#include <sigmafive/config.hpp>

#include <array>
#include <boost/serialization/access.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/vector.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <sigmafive/resource/identifier.hpp>
#include <sigmafive/util/glm_serialize.hpp>
#include <string>

namespace sigmafive {
namespace graphics {
    enum class shader_type {
        none,
        vertex,
        fragment
    };

    class SIGMAFIVE_API shader {
    public:
        shader(shader_type type = shader_type::none, std::string code = "");

        shader(const shader&) = delete;

        shader(shader&&) = default;

        shader& operator=(const shader&) = delete;

        shader& operator=(shader&&) = default;

        ~shader() = default;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& type;
            ar& source;
        }

        shader_type type = shader_type::none;
        std::string source;
    };

    class SIGMAFIVE_API shader_cache {
    public:
        shader_cache() = default;

        shader_cache(shader_cache&&) noexcept = default;

        shader_cache(const shader_cache&) = delete;

        shader_cache& operator=(shader_cache&&) noexcept = default;

        shader_cache& operator=(const shader_cache&) = delete;

        virtual ~shader_cache() = default;

        /**
        * @brief Returns if shader is loaded in this cache.
        *
        * @param shader the shader to check if cached.
        * @return true if the shader is cache.
        */
        virtual bool is_cached(resource::identifier shader) const = 0;

        /**
        * @brief Increases the reference count associated with the shader.
        *
        *
        * @param shader the shader to increase the reference count of.
        * @return true if the shader exists and is valid.
        */
        virtual bool increment_reference(resource::identifier shader) = 0;

        /**
        * @brief Decreases the reference count associated with
        * the shader.
        *
        *
        * @param shader the shader to decrease the reference count of.
        * @returns true if the shader reference count is zero.
        */
        virtual bool decrement_reference(resource::identifier shader) = 0;
    };
}
}

#endif // SIGMAFIVE_ENGINE_GRAPHICS_SHADER_HPP
