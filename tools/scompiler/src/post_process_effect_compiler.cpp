#include <compiler_util.hpp>
#include <post_process_effect_compiler.hpp>

#include <shader_technique_compiler.hpp>

#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/util/filesystem.hpp>

#include <json/json.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>

#include <fstream>
#include <iostream>

namespace sigma {
bool is_post_process_effect(boost::filesystem::path file)
{
    return file.extension() == ".eff";
}

bool compile_post_process_effects(boost::filesystem::path outputdir, boost::filesystem::path sourcedir, std::vector<boost::filesystem::path> post_process_effects)
{
    bool all_good = true;
    for (const auto& file_path : post_process_effects) {
        try {
            std::ifstream file{ file_path.string(), std::ios::in };
            Json::Value json_effect;
            file >> json_effect;

            sigma::graphics::post_process_effect effect;
            auto rid = boost::filesystem::path{ "post_process_effect" } / sigma::filesystem::make_relative(sourcedir, file_path).replace_extension("");

            compile_shader_technique(effect, json_effect);

            if (effect.shaders.count(graphics::shader_type::vertex) == 0)
                effect.shaders[graphics::shader_type::vertex] = boost::filesystem::path{ "vertex" } / "fullscreen_quad";

            if (json_effect.isMember("static_mesh"))
                effect.mesh = boost::filesystem::path{ "static_mesh" } / json_effect["static_mesh"].asString();

            auto final_path = outputdir / rid;

            auto output_folder = final_path.parent_path();
            if (!boost::filesystem::exists(output_folder))
                boost::filesystem::create_directories(output_folder);

            std::ofstream stream{ final_path.string(), std::ios::binary };
            boost::archive::binary_oarchive oa(stream);
            oa << effect;

            touch_stamp_file(outputdir, file_path);
        } catch (...) { // TODO cacth the correct error types and get the messages
            all_good = false;
            std::cerr << "scompiler: error: could not compile post_process_effect '" << file_path << "'!\n";
        }
    }
    return all_good;
}
}
