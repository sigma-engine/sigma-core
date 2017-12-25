#ifndef SIGMA_PACKAGE_TECHNIQUE_HPP
#define SIGMA_PACKAGE_TECHNIQUE_HPP

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/database.hpp>
#include <sigma/util/json_conversion.hpp>

#include <boost/filesystem/path.hpp>

template <class Effect>
void convert_effect(
    const sigma::resource::database<sigma::graphics::texture>& texture_database,
    const sigma::resource::database<sigma::graphics::cubemap>& cubemap_database,
    const sigma::resource::database<sigma::graphics::technique>& technique_database,
    Effect& effect,
    const Json::Value& settings)
{
    std::vector<boost::filesystem::path> cid;
    if (settings.isMember("vertex"))
        cid.push_back(boost::filesystem::path{ "vertex" } / settings["vertex"].asString());

    if (settings.isMember("tessellation_control"))
        cid.push_back(boost::filesystem::path{ "tessellation_control" } / settings["tessellation_control"].asString());

    if (settings.isMember("tessellation_evaluation"))
        cid.push_back(boost::filesystem::path{ "tessellation_evaluation" } / settings["tessellation_evaluation"].asString());

    if (settings.isMember("geometry"))
        cid.push_back(boost::filesystem::path{ "geometry" } / settings["geometry"].asString());

    if (settings.isMember("fragment"))
        cid.push_back(boost::filesystem::path{ "fragment" } / settings["fragment"].asString());

    effect.technique_id = technique_database.handle_for(cid);

    // TODO error if missing texture or cubemap.
    for (auto it = settings.begin(); it != settings.end(); ++it) {
        const auto& value = *it;
        if (it.key() == "vertex" || it.key() == "tessellation_control" || it.key() == "tessellation_evaluation" || it.key() == "geometry" || it.key() == "fragment") {
            continue;
        } else if (it.key() == "textures") {
            const auto& texture_object = *it;
            for (auto it2 = texture_object.begin(); it2 != texture_object.end(); ++it2)
                effect.textures[it2.key().asString()] = texture_database.handle_for({ boost::filesystem::path{ "texture" } / (*it2).asString() });
        } else if (it.key() == "cubemaps") {
            const auto& cubemap_object = *it;
            for (auto it2 = cubemap_object.begin(); it2 != cubemap_object.end(); ++it2)
                effect.cubemaps[it2.key().asString()] = cubemap_database.handle_for({ boost::filesystem::path{ "cubemap" } / (*it2).asString() }); // TODO warn if tring to set cubemap more than once
        } else {
            float f;
            glm::vec2 v2;
            glm::vec3 v3;
            glm::vec4 v4;
            if (sigma::json::from_json(value, v4))
                effect.vec4s[it.key().asString()] = v4;
            else if (sigma::json::from_json(value, v3))
                effect.vec3s[it.key().asString()] = v3;
            else if (sigma::json::from_json(value, v2))
                effect.vec2s[it.key().asString()] = v2;
            else if (sigma::json::from_json(value, f))
                effect.floats[it.key().asString()] = f;
        }
    }
}

void package_techniques(
    const sigma::resource::database<sigma::graphics::shader>& shader_database,
    sigma::resource::database<sigma::graphics::technique>& technique_database,
    const boost::filesystem::path& source_directory,
    const std::vector<boost::filesystem::path>& include_directories);

#endif // SIGMA_PACKAGE_TECHNIQUE_HPP
