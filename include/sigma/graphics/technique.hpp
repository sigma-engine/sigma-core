#ifndef SIGMA_GRAPHICS_TECHNIQUE_HPP
#define SIGMA_GRAPHICS_TECHNIQUE_HPP

#include <sigma/resource/manager.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/version.hpp>

#include <set>
#include <string>
#include <unordered_map>

namespace sigma {
namespace graphics {
    struct technique_identifier {
        boost::filesystem::path vertex;
        boost::filesystem::path tessellation_control;
        boost::filesystem::path tessellation_evaluation;
        boost::filesystem::path geometry;
        boost::filesystem::path fragment;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& vertex;
            ar& tessellation_control;
            ar& tessellation_evaluation;
            ar& geometry;
            ar& fragment;
        }

        bool operator==(const technique_identifier& rhs) const
        {
            return vertex == rhs.vertex && tessellation_control == rhs.tessellation_control && tessellation_evaluation == rhs.tessellation_evaluation && geometry == rhs.geometry && fragment == rhs.fragment;
        }
    };

    struct technique_uniform_data {
        std::unordered_map<std::string, float> floats;
        std::unordered_map<std::string, glm::vec2> vec2s;
        std::unordered_map<std::string, glm::vec3> vec3s;
        std::unordered_map<std::string, glm::vec4> vec4s;
        std::unordered_map<std::string, boost::filesystem::path> textures;
        std::unordered_map<std::string, boost::filesystem::path> cubemaps;

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
        technique_identifier shaders;
        std::set<std::string> float_uniforms;
        std::set<std::string> vec2_uniforms;
        std::set<std::string> vec3_uniforms;
        std::set<std::string> vec4_uniforms;
        std::set<std::string> texture_uniforms;
        std::set<std::string> cubemap_uniforms;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& shaders;
            ar& float_uniforms;
            ar& vec2_uniforms;
            ar& vec3_uniforms;
            ar& vec4_uniforms;
            ar& texture_uniforms;
            ar& cubemap_uniforms;
        }
    };
}
namespace resource {
    template <>
    struct resource_loader<graphics::technique> {
        typedef graphics::technique_identifier identifier_type;
        std::unique_ptr<graphics::technique> operator()(const boost::filesystem::path& cache_directory, const identifier_type& id) const
        {
            // TODO load from file
            auto tech = std::make_unique<graphics::technique>();
            tech->shaders = id;
            return tech;
        }
    };
}
}

namespace std {
template <>
struct hash<sigma::graphics::technique_identifier> {
    size_t operator()(const sigma::graphics::technique_identifier& id) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, id.vertex);
        boost::hash_combine(seed, id.tessellation_control);
        boost::hash_combine(seed, id.tessellation_evaluation);
        boost::hash_combine(seed, id.geometry);
        boost::hash_combine(seed, id.fragment);
        return seed;
    }
};
}

BOOST_CLASS_VERSION(sigma::graphics::technique_identifier, 1);
BOOST_CLASS_VERSION(sigma::graphics::technique_uniform_data, 1);
BOOST_CLASS_VERSION(sigma::graphics::technique, 1);

#endif // SIGMA_GRAPHICS_TECHNIQUE_HPP
