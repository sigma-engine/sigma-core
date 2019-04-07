#ifndef SIGMA_GRAPHICS_TECHNIQUE_HPP
#define SIGMA_GRAPHICS_TECHNIQUE_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/reflect.hpp>
#include <sigma/resource/cache.hpp>
#include <sigma/resource/resource.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <set>
#include <string>
#include <unordered_map>

namespace sigma {
namespace graphics {
    struct technique_identifier {
        std::filesystem::path vertex;
        std::filesystem::path tessellation_control;
        std::filesystem::path tessellation_evaluation;
        std::filesystem::path geometry;
        std::filesystem::path fragment;

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(vertex,
            tessellation_control,
            tessellation_evaluation,
            geometry,
            fragment);
        }
    };

    struct technique_uniform_data {
        std::unordered_map<std::string, float> floats;
        std::unordered_map<std::string, glm::vec2> vec2s;
        std::unordered_map<std::string, glm::vec3> vec3s;
        std::unordered_map<std::string, glm::vec4> vec4s;
        std::unordered_map<std::string, resource::handle<texture>> textures;
        std::unordered_map<std::string, resource::handle<cubemap>> cubemaps;

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(floats,
            vec2s,
            vec3s,
            vec4s,
            textures,
            cubemaps);
        }
    };

    struct technique {
        resource::handle<shader> vertex;
        resource::handle<shader> tessellation_control;
        resource::handle<shader> tessellation_evaluation;
        resource::handle<shader> geometry;
        resource::handle<shader> fragment;

        std::set<std::string> float_uniforms;
        std::set<std::string> vec2_uniforms;
        std::set<std::string> vec3_uniforms;
        std::set<std::string> vec4_uniforms;
        std::set<std::string> texture_uniforms;
        std::set<std::string> cubemap_uniforms;

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(vertex,
            tessellation_control,
            tessellation_evaluation,
            geometry,
            fragment,
            float_uniforms,
            vec2_uniforms,
            vec3_uniforms,
            vec4_uniforms,
            texture_uniforms,
            cubemap_uniforms);
        }
    };
}
}

REGISTER_RESOURCE(sigma::graphics::technique, technique, 1)

#endif // SIGMA_GRAPHICS_TECHNIQUE_HPP
