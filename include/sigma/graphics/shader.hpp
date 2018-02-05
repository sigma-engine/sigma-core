#ifndef SIGMA_GRAPHICS_SHADER_HPP
#define SIGMA_GRAPHICS_SHADER_HPP

#include <sigma/config.hpp>
#include <sigma/resource/resource.hpp>

#include <boost/serialization/vector.hpp>

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
        std::vector<std::uint32_t> spirv;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& type;
            ar& source;
            if (version > 1)
                ar& spirv;
        }
    };
}
}

REGISTER_RESOURCE(sigma::graphics::shader, shader, 2)

#endif // SIGMA_GRAPHICS_SHADER_HPP
