#ifndef SIGMA_TOOLS_EFFCC_MATERIAL_COMPILE_HPP
#define SIGMA_TOOLS_EFFCC_MATERIAL_COMPILE_HPP

#include <technique_compile.hpp>

#include <sigma/graphics/material.hpp>
#include <sigma/graphics/texture.hpp>

auto compile_material(const sigma::resource::database<sigma::graphics::texture>& texture_database, const sigma::resource::database<sigma::graphics::cubemap>& cubemap_database, const sigma::resource::database<sigma::graphics::shader>& shader_database, sigma::resource::database<sigma::graphics::technique>& technique_database, const Json::Value& settings)
{
    sigma::graphics::material material;

    std::vector<boost::filesystem::path> technique_cid;
    sigma::graphics::technique technique;
    std::tie(technique_cid, technique) = compile_technique(shader_database, settings);
    material.technique_id = technique_database.insert(technique_cid, technique);

    for (auto it = settings.begin(); it != settings.end(); ++it) {
        const auto& value = *it;
        if (it.key() == "vertex" || it.key() == "tessellation_control" || it.key() == "tessellation_evaluation" || it.key() == "geometry" || it.key() == "fragment") {
            continue;
        } else if (it.key() == "textures") {
            const auto& texture_object = *it;
            for (auto it2 = texture_object.begin(); it2 != texture_object.end(); ++it2)
                material.textures[it2.key().asString()] = texture_database.handle_for({ boost::filesystem::path{ "texture" } / (*it2).asString() });
        } else if (it.key() == "cubemaps") {
            const auto& cubemap_object = *it;
            for (auto it2 = cubemap_object.begin(); it2 != cubemap_object.end(); ++it2)
                material.cubemaps[it2.key().asString()] = cubemap_database.handle_for({ boost::filesystem::path{ "cubemap" } / (*it2).asString() }); // TODO warn if tring to set cubemap more than once
        } else {
            float f;
            glm::vec2 v2;
            glm::vec3 v3;
            glm::vec4 v4;
            if (sigma::json::from_json(value, v4))
                material.vec4s[it.key().asString()] = v4;
            else if (sigma::json::from_json(value, v3))
                material.vec3s[it.key().asString()] = v3;
            else if (sigma::json::from_json(value, v2))
                material.vec2s[it.key().asString()] = v2;
            else if (sigma::json::from_json(value, f))
                material.floats[it.key().asString()] = f;
        }
    }
    return material;
}

#endif // SIGMA_TOOLS_EFFCC_MATERIAL_COMPILE_HPP
