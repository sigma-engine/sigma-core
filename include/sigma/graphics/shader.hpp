#ifndef SIGMA_GRAPHICS_SHADER_HPP
#define SIGMA_GRAPHICS_SHADER_HPP

#include <sigma/config.hpp>
#include <sigma/reflect/reflect.hpp>
#include <sigma/resource/identifier.hpp>
#include <sigma/resource/resource_cache.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/vector.hpp>

#include <array>
#include <string>

namespace sigma {
namespace graphics {
    enum class shader_type {
        none,
        vertex,
        fragment
    };

    RCLASS()
    class SIGMA_API shader {
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

        friend class resource::resource_cache<shader>;
        std::size_t reference_count = 0;
    };

    using shader_cache = resource::resource_cache<shader>;
}
}

#endif // SIGMA_GRAPHICS_SHADER_HPP
