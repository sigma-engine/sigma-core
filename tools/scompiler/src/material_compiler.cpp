#include <compiler_util.hpp>
#include <material_compiler.hpp>
#include <shader_technique_compiler.hpp>

#include <sigma/graphics/material.hpp>
#include <sigma/util/filesystem.hpp>

#include <json/json.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>
#include <iostream>

namespace sigma {
bool is_material(boost::filesystem::path file)
{
    return file.extension() == ".smat";
}

bool compile_materials(boost::filesystem::path outputdir, boost::filesystem::path sourcedir, std::vector<boost::filesystem::path> materials)
{
    bool all_good = true;
    for (const auto& file_path : materials) {
        try {
            std::ifstream file{ file_path.string(), std::ios::in };
            Json::Value json_material;
            file >> json_material;

            sigma::graphics::material material;
            auto rid = boost::filesystem::path{ "material" } / sigma::filesystem::make_relative(sourcedir, file_path).replace_extension("");

            compile_shader_technique(material, json_material);

            if (json_material.isMember("transparent"))
                material.transparent = json_material["transparent"].asBool();

            auto final_path = outputdir / rid;

            auto output_folder = final_path.parent_path();
            if (!boost::filesystem::exists(output_folder))
                boost::filesystem::create_directories(output_folder);

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
