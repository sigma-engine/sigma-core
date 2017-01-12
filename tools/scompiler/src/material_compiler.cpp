#include <compiler_util.hpp>
#include <material_compiler.hpp>
#include <shader_technique_compiler.hpp>

#include <sigma/graphics/material.hpp>

#include <json/json.h>

#include <boost/archive/binary_oarchive.hpp>

namespace sigma {
bool is_material(boost::filesystem::path file)
{
    return file.extension() == ".mat";
}

bool compile_materials(boost::filesystem::path outputdir, std::vector<boost::filesystem::path> materials)
{
    bool all_good = true;
    for (const auto& file_path : materials) {
        if (!resource_has_changes(outputdir, file_path))
            continue;

        std::cout << file_path.filename().string() << std::endl;
        try {
            std::ifstream file{ file_path.string(), std::ios::in };
            Json::Value json_material;
            file >> json_material;

            sigma::graphics::material_data material;
            sigma::resource::development_identifier rid("material", file_path);

            compile_shader_technique(material, json_material);

            if (json_material.isMember("transparent"))
                material.transparent = json_material["transparent"].asBool();

            auto final_path = outputdir / std::to_string(rid.value());
            std::ofstream stream{ final_path.string(), std::ios::binary };
            boost::archive::binary_oarchive oa(stream);
            oa << material;

            touch_stamp_file(outputdir, file_path);
        } catch (...) { // TODO cacth the correct error types and get the messages
            all_good = false;
            std::cerr << "scompiler: error: could not compile material '" << file_path << "'!\n";
        }
    }
    return all_good;
}
}
