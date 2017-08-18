#ifndef SIGMA_GRAPHICS_SHADER_HPP
#define SIGMA_GRAPHICS_SHADER_HPP

#include <sigma/config.hpp>

#include <boost/serialization/version.hpp>

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

BOOST_CLASS_VERSION(sigma::graphics::shader, 1);

#endif // SIGMA_GRAPHICS_SHADER_HPP
