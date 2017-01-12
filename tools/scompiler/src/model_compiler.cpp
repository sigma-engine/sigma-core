#include <assimp_converter.hpp>
#include <compiler_util.hpp>
#include <model_compiler.hpp>

#include <sigma/graphics/static_mesh.hpp>
#include <sigma/util/filesystem.hpp>

#include <json/json.h>

#include <boost/archive/binary_oarchive.hpp>

#include <iostream>
#include <vector>

namespace sigma {
static std::vector<std::string> exts{
    ".3ds",
    ".blend",
    ".dae",
    ".fbx",
    ".ifc-step",
    ".ase",
    ".dxf",
    ".hmp",
    ".md2",
    ".md3",
    ".md5",
    ".mdc",
    ".mdl",
    ".nff",
    ".ply",
    ".stl",
    ".x",
    ".obj",
    ".opengex",
    ".smd",
    ".lwo",
    ".lxo",
    ".lws",
    ".ter",
    ".ac3d",
    ".ms3d",
    ".cob",
    ".q3bsp",
    ".xgl",
    ".csm",
    ".bvh",
    ".b3d",
    ".ndo",
    ".q3d",
    ".assbin",
    ".gltf",
    ".3mf"
};

bool is_model(boost::filesystem::path file)
{
    return std::find(exts.begin(), exts.end(), file.extension().string()) != exts.end();
}

bool compile_models(boost::filesystem::path outputdir, std::vector<boost::filesystem::path> models)
{
    bool all_good = true;
    for (auto file_path : models) {
        if (!resource_has_changes(outputdir, file_path))
            continue;

        std::cout << file_path.filename().string() << std::endl;
        try {
            sigma::assimp_converter imported{ file_path };
            for (auto mesh_name : imported.static_mesh_names()) {
                sigma::resource::development_identifier rid("static_mesh", file_path, mesh_name);
                auto final_path = outputdir / std::to_string(rid.value());

                sigma::graphics::static_mesh_data mesh;
                imported.convert_static_mesh(mesh_name, mesh);

                std::ofstream stream(final_path.string(), std::ios::binary | std::ios::out);
                boost::archive::binary_oarchive oa(stream);
                oa << mesh;
            }

            auto scene_path = file_path;
            scene_path.replace_extension(".scn");

            Json::Value json_scene;
            if (boost::filesystem::exists(scene_path)) {
                std::ifstream file(scene_path.string());
                file >> json_scene;
            }

            for (auto object_name : imported.scene_object_names())
                imported.convert_object(object_name, json_scene[object_name]);

            scene_path = sigma::filesystem::make_relative(boost::filesystem::current_path(), scene_path);
            scene_path = outputdir / scene_path;
            std::ofstream outscene(scene_path.string());
            outscene << json_scene;

            touch_stamp_file(outputdir, file_path);
        } catch (...) { // TODO cacth the correct error types and get the messages
            all_good = false;
            std::cerr << "scompiler: error: could not compile model '" << file_path << "'!\n";
        }
    }
    return all_good;
}
}
