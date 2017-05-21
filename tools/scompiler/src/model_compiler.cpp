#include <assimp_converter.hpp>
#include <compiler_util.hpp>
#include <material_compiler.hpp>
#include <model_compiler.hpp>

#include <sigma/graphics/static_mesh.hpp>
#include <sigma/util/filesystem.hpp>

#include <json/json.h>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <fstream>
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
            sigma::assimp_converter imported{ boost::filesystem::current_path(), file_path };

            std::vector<boost::filesystem::path> material_paths;
            for (auto material_name : imported.material_names()) {
                boost::filesystem::path material_path = boost::filesystem::current_path() / material_name;
                material_path.replace_extension("smat");
                material_paths.push_back(material_path);

                Json::Value json_material(Json::objectValue);
                if (boost::filesystem::exists(material_path)) {
                    std::ifstream file(material_path.string());
                    file >> json_material;
                }

                imported.convert_material(material_name, json_material);

                std::ofstream file(material_path.string());
                file << json_material;
            }

            compile_materials(outputdir, material_paths);

            for (auto mesh_name : imported.static_mesh_names()) {
                if (!boost::algorithm::ends_with(mesh_name, "_high")) {
                    sigma::resource::identifier rid("static_mesh", file_path, mesh_name);
                    auto final_path = outputdir / std::to_string(rid.value());

                    sigma::graphics::static_mesh mesh;
                    imported.convert_static_mesh(mesh_name, mesh);

                    std::ofstream stream(final_path.string(), std::ios::binary | std::ios::out);
                    boost::archive::binary_oarchive oa(stream);
                    oa << mesh;
                }
            }

            auto scene_path = file_path;
            scene_path.replace_extension(".scn");

            Json::Value json_scene(Json::objectValue);
            if (boost::filesystem::exists(scene_path)) {
                std::ifstream file(scene_path.string());
                file >> json_scene;
            }

            for (auto object_name : imported.scene_object_names()) {
                if (!boost::algorithm::ends_with(object_name, "_high")) {
                    imported.convert_object(object_name, json_scene[object_name]);
                }
            }

            auto full_scene_path = sigma::filesystem::make_relative(boost::filesystem::current_path(), scene_path);
            full_scene_path = outputdir / full_scene_path;
            std::ofstream outscene(full_scene_path.string());
            outscene << json_scene;
            if (boost::filesystem::exists(scene_path))
                touch_stamp_file(outputdir, file_path, { scene_path });
            else
                touch_stamp_file(outputdir, file_path);
        } catch (...) { // TODO cacth the correct error types and get the messages
            all_good = false;
            std::cerr << "scompiler: error: could not compile model '" << file_path << "'!\n";
        }
    }
    return all_good;
}
}
