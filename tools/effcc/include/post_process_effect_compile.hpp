#ifndef SIGMA_TOOLS_EFFCC_POST_PROCESS_EFFECT_COMPILE_HPP
#define SIGMA_TOOLS_EFFCC_POST_PROCESS_EFFECT_COMPILE_HPP

#include <technique_compile.hpp>

#include <sigma/graphics/post_process_effect.hpp>

auto compile_post_process_effect(const sigma::resource::database<sigma::graphics::texture>& texture_database, const sigma::resource::database<sigma::graphics::cubemap>& cubemap_database, const sigma::resource::database<sigma::graphics::shader>& shader_database, sigma::resource::database<sigma::graphics::technique>& technique_database, const sigma::resource::database<sigma::graphics::static_mesh>& static_mesh_database, Json::Value& settings)
{
    if (!settings.isMember("static_mesh"))
        settings["static_mesh"] = "fullscreen_quad";

    if (!settings.isMember("vertex"))
        settings["vertex"] = "fullscreen_quad";

    std::vector<boost::filesystem::path> technique_cid;
    sigma::graphics::technique technique;
    std::tie(technique_cid, technique) = compile_technique(shader_database, settings);

    sigma::graphics::post_process_effect effect;
    effect.technique_id = technique_database.insert(technique_cid, technique);

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
        } else if (it.key() == "static_mesh") {
            effect.mesh = static_mesh_database.handle_for({ boost::filesystem::path{ "static_mesh" } / value.asString() });
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

    return effect;
}

#endif // SIGMA_TOOLS_EFFCC_POST_PROCESS_EFFECT_COMPILE_HPP
