#ifndef SIGMA_GRAPHICS_SHADER_TECHNIQUE_HPP
#define SIGMA_GRAPHICS_SHADER_TECHNIQUE_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/resource/identifier.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/version.hpp>

#include <cstddef>
#include <string>
#include <unordered_map>

namespace sigma {
namespace graphics {

    struct shader_technique {
        std::unordered_map<std::string, float> floats;
        std::unordered_map<std::string, glm::vec2> vec2s;
        std::unordered_map<std::string, glm::vec3> vec3s;
        std::unordered_map<std::string, glm::vec4> vec4s;
        std::unordered_map<shader_type, resource::identifier> shaders;
        std::unordered_map<std::string, resource::identifier> textures;
        std::unordered_map<std::string, resource::identifier> cubemaps;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& floats;
            ar& vec2s;
            ar& vec3s;
            ar& vec4s;
            ar& shaders;
            ar& textures;
            ar& cubemaps;
        }
    };
}
}

BOOST_CLASS_VERSION(sigma::graphics::shader_technique, 1);

#endif // SIGMA_GRAPHICS_SHADER_TECHNIQUE_HPP
