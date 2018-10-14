#ifndef SIGMA_GRAPHICS_TECHNIQUE_HPP
#define SIGMA_GRAPHICS_TECHNIQUE_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/reflect.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <set>
#include <string>
#include <unordered_map>

namespace sigma {
namespace graphics {
    struct technique_uniform_data {
        std::unordered_map<std::string, float> floats;
        std::unordered_map<std::string, glm::vec2> vec2s;
        std::unordered_map<std::string, glm::vec3> vec3s;
        std::unordered_map<std::string, glm::vec4> vec4s;
        std::unordered_map<std::string, std::shared_ptr<texture>> textures;
        std::unordered_map<std::string, std::shared_ptr<cubemap>> cubemaps;
    };

    struct technique {
        std::shared_ptr<shader> vertex;
        std::shared_ptr<shader> tessellation_control;
        std::shared_ptr<shader> tessellation_evaluation;
        std::shared_ptr<shader> geometry;
        std::shared_ptr<shader> fragment;

        std::set<std::string> float_uniforms;
        std::set<std::string> vec2_uniforms;
        std::set<std::string> vec3_uniforms;
        std::set<std::string> vec4_uniforms;
        std::set<std::string> texture_uniforms;
        std::set<std::string> cubemap_uniforms;
    };
}
}

#endif // SIGMA_GRAPHICS_TECHNIQUE_HPP
