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

#include <boost/serialization/set.hpp>
#include <boost/serialization/unordered_map.hpp>

#include <set>
#include <string>
#include <unordered_map>

namespace sigma {
namespace graphics {
    struct technique_identifier {
        // TODO this only place this is used is in the tools
        BOOST_HANA_DEFINE_STRUCT(
            technique_identifier,
            (boost::filesystem::path, vertex),
            (boost::filesystem::path, tessellation_control),
            (boost::filesystem::path, tessellation_evaluation),
            (boost::filesystem::path, geometry),
            (boost::filesystem::path, fragment));

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& vertex;
            ar& tessellation_control;
            ar& tessellation_evaluation;
            ar& geometry;
            ar& fragment;
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
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& floats;
            ar& vec2s;
            ar& vec3s;
            ar& vec4s;
            ar& textures;
            ar& cubemaps;
        }
    };

    struct technique {
        // TODO put the shaders in here directly.
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
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& vertex;
            ar& tessellation_control;
            ar& tessellation_evaluation;
            ar& geometry;
            ar& fragment;

            ar& float_uniforms;
            ar& vec2_uniforms;
            ar& vec3_uniforms;
            ar& vec4_uniforms;
            ar& texture_uniforms;
            ar& cubemap_uniforms;
        }
    };
}
}

BOOST_CLASS_VERSION(sigma::graphics::technique_identifier, 1)
BOOST_CLASS_VERSION(sigma::graphics::technique_uniform_data, 1)
REGISTER_RESOURCE(sigma::graphics::technique, technique, 1)

#endif // SIGMA_GRAPHICS_TECHNIQUE_HPP
