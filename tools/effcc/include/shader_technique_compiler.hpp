#ifndef SIGMA_SHADER_TECHNIQUE_COMPILER_HPP
#define SIGMA_SHADER_TECHNIQUE_COMPILER_HPP

#include <sigma/graphics/technique.hpp>
#include <sigma/util/json_conversion.hpp>

#include <json/json.h>

namespace sigma {
template <class T>
void compile_shader_technique(T& technique, const Json::Value& technique_data)
{
    for (auto it = technique_data.begin(); it != technique_data.end(); ++it) {
        const auto& value = *it;
        if (it.key() == "vertex") {
            technique.technique_id.vertex = boost::filesystem::path{ "vertex" } / value.asString(); // TODO warn if tring to set shader more that once
        } else if (it.key() == "tessellation_control") {
            technique.technique_id.tessellation_control = boost::filesystem::path{ "tessellation_control" } / value.asString(); // TODO warn if tring to set shader more that once
        } else if (it.key() == "tessellation_evaluation") {
            technique.technique_id.tessellation_evaluation = boost::filesystem::path{ "tessellation_evaluation" } / value.asString(); // TODO warn if tring to set shader more that once
        } else if (it.key() == "geometry") {
            technique.technique_id.geometry = boost::filesystem::path{ "geometry" } / value.asString(); // TODO warn if tring to set shader more that once
        } else if (it.key() == "fragment") {
            technique.technique_id.fragment = boost::filesystem::path{ "fragment" } / value.asString(); // TODO warn if tring to set shader more that once
        } else if (it.key() == "textures") {
            const auto& texture_object = *it;
            for (auto it2 = texture_object.begin(); it2 != texture_object.end(); ++it2)
                technique.textures[it2.key().asString()] = boost::filesystem::path{ "texture" } / (*it2).asString(); // TODO warn if tring to set texture more than once
        } else if (it.key() == "cubemaps") {
            const auto& cubemap_object = *it;
            for (auto it2 = cubemap_object.begin(); it2 != cubemap_object.end(); ++it2)
                technique.cubemaps[it2.key().asString()] = boost::filesystem::path{ "cubemap" } / (*it2).asString(); // TODO warn if tring to set cubemap more than once
        } else {
            float f;
            glm::vec2 v2;
            glm::vec3 v3;
            glm::vec4 v4;
            if (sigma::json::from_json(value, v4))
                technique.vec4s[it.key().asString()] = v4;
            else if (sigma::json::from_json(value, v3))
                technique.vec3s[it.key().asString()] = v3;
            else if (sigma::json::from_json(value, v2))
                technique.vec2s[it.key().asString()] = v2;
            else if (sigma::json::from_json(value, f))
                technique.floats[it.key().asString()] = f;
        }
    }

    // TODO check for errors like no vertex or fragment shader
}
}

#endif // SIGMA_SHADER_TECHNIQUE_COMPILER_HPP
