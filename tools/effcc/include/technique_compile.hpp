#ifndef SIGMA_TOOLS_EFFCC_TECHNIQUE_COMPILE_HPP
#define SIGMA_TOOLS_EFFCC_TECHNIQUE_COMPILE_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/resource/database.hpp>

auto compile_technique(const sigma::resource::database<sigma::graphics::shader>& shader_database, const Json::Value& settings)
{
    sigma::graphics::technique technique;
    std::vector<boost::filesystem::path> cid;

    // TODO check for errors like no vertex or fragment shader
    if (settings.isMember("vertex")) {
        cid.push_back(boost::filesystem::path{ "vertex" } / settings["vertex"].asString());
        technique.shaders.vertex = shader_database.handle_for({ cid.back() });
    }
    if (settings.isMember("tessellation_control")) {
        cid.push_back(boost::filesystem::path{ "tessellation_control" } / settings["tessellation_control"].asString());
        technique.shaders.tessellation_control = shader_database.handle_for({ cid.back() });
    }
    if (settings.isMember("tessellation_evaluation")) {
        cid.push_back(boost::filesystem::path{ "tessellation_evaluation" } / settings["tessellation_evaluation"].asString());
        technique.shaders.tessellation_evaluation = shader_database.handle_for({ cid.back() });
    }
    if (settings.isMember("geometry")) {
        cid.push_back(boost::filesystem::path{ "geometry" } / settings["geometry"].asString());
        technique.shaders.geometry = shader_database.handle_for({ cid.back() });
    }
    if (settings.isMember("fragment")) {
        cid.push_back(boost::filesystem::path{ "fragment" } / settings["fragment"].asString());
        technique.shaders.fragment = shader_database.handle_for({ cid.back() });
    }

    return std::make_pair(cid, technique);
}

#endif // SIGMA_TOOLS_EFFCC_TECHNIQUE_COMPILE_HPP
