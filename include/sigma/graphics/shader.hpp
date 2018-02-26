#ifndef SIGMA_GRAPHICS_SHADER_HPP
#define SIGMA_GRAPHICS_SHADER_HPP

#include <sigma/config.hpp>
#include <sigma/resource/resource.hpp>

#include <boost/serialization/vector.hpp>

#include <vector>

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
        std::vector<unsigned char> spirv;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& type;
            ar& spirv;
        }
    };
}
}

REGISTER_RESOURCE(sigma::graphics::shader, shader, 1)

#endif // SIGMA_GRAPHICS_SHADER_HPP
