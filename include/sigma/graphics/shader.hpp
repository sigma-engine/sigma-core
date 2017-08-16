#ifndef SIGMA_GRAPHICS_SHADER_HPP
#define SIGMA_GRAPHICS_SHADER_HPP

#include <sigma/config.hpp>

#include <boost/serialization/version.hpp>

#include <array>
#include <string>

namespace sigma {
namespace graphics {
    enum class shader_type : size_t {
        vertex,
        tessellation_control,
        tessellation_evaluation,
        geometry,
        fragment,
        header
    };

    static constexpr std::array<shader_type, 3> all_shader_types()
    {
        return { shader_type::vertex, shader_type::fragment, shader_type::geometry };
    }

    struct shader {
        shader_type type;
        std::string source;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& type;
            ar& source;
        }
    };
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

BOOST_CLASS_VERSION(sigma::graphics::shader, 1);

#endif // SIGMA_GRAPHICS_SHADER_HPP
