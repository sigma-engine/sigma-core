#include <compiler_util.hpp>
#include <post_process_effect_compiler.hpp>

#include <shader_technique_compiler.hpp>

#include <sigma/graphics/post_process_effect.hpp>

#include <json/json.h>

#include <boost/archive/binary_oarchive.hpp>

#include <fstream>

namespace sigma {
bool is_post_process_effect(boost::filesystem::path file)
{
    return file.extension() == ".eff";
}

bool compile_post_process_effects(boost::filesystem::path outputdir, std::vector<boost::filesystem::path> post_process_effects)
{
    bool all_good = true;
    for (const auto& file_path : post_process_effects) {
        if (!resource_has_changes(outputdir, file_path))
            continue;
        std::cout << file_path.filename().string() << std::endl;
        try {
            std::ifstream file{ file_path.string(), std::ios::in };
            Json::Value json_effect;
            file >> json_effect;

            sigma::graphics::post_process_effect effect;
            sigma::resource::identifier rid("post_process_effect", file_path);

            compile_shader_technique(effect, json_effect);

            if (effect.shaders.count(graphics::shader_type::vertex) == 0)
                effect.shaders[graphics::shader_type::vertex] = resource::identifier{ "vertex", "fullscreen_quad" };

            if (json_effect.isMember("static_mesh"))
                effect.mesh = sigma::resource::identifier{ "static_mesh", json_effect["static_mesh"].asString() };

            auto final_path = outputdir / std::to_string(rid.value());
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
