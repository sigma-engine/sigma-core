#ifndef SIGMA_GRAPHICS_SHADER_HPP
#define SIGMA_GRAPHICS_SHADER_HPP

#include <sigma/config.hpp>
#include <sigma/reflect/reflect.hpp>
#include <sigma/resource/cache.hpp>
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
    enum class shader_type : size_t {
        none,
        vertex,
        fragment,
        geometry
    };

    RCLASS()
    class SIGMA_API shader {
    public:
        shader(shader_type type = shader_type::none, std::string code = "");

        shader(shader&&) = default;

        shader& operator=(shader&&) = default;

        ~shader() = default;

        // TODO make these private
        shader_type type = shader_type::none;
        std::string source;

    private:
        shader(const shader&) = delete;
        shader& operator=(const shader&) = delete;

        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& type;
            ar& source;
        }
    };

    using shader_cache = resource::cache<shader>;

}
}

namespace std {
template <>
struct hash<sigma::graphics::shader_type> {
    size_t operator()(const sigma::graphics::shader_type& type) const
    {
        return size_t(type);
    }
};
}

#endif // SIGMA_GRAPHICS_SHADER_HPP
