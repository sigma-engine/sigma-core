#include "assimp_converter.hpp"

#include <sigma/graphics/static_mesh.hpp>
#include <sigma/util/filesystem.hpp>

#include <json/json.h>

#include <boost/algorithm/string.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char const* argv[])
{
    po::options_description global_options("Options");
    global_options.add_options()("help,h", "Show this help message")("output,o", po::value<boost::filesystem::path>()->default_value(boost::filesystem::current_path()), "output directory")("input-files", po::value<std::vector<boost::filesystem::path>>(), "input model files");

    po::positional_options_description positional_options;
    positional_options.add("input-files", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(global_options).positional(positional_options).run(), vm);

    if (vm.count("help")) {
        std::cout << global_options << std::endl;
        return -1;
    }

    if (vm.count("input-files") <= 0) {
        std::cerr << "scmodel: fatal error: no input files." << std::endl;
        return -1;
    }

    auto outputdir = vm["output"].as<boost::filesystem::path>();
    boost::filesystem::create_directories(outputdir);

    for (auto file_path :
        vm["input-files"].as<std::vector<boost::filesystem::path>>()) {
        file_path = boost::filesystem::absolute(file_path);
        if (sigma::filesystem::contains_file(boost::filesystem::current_path(), file_path)) {
            if (boost::filesystem::exists(file_path)) {
                std::cout << "Compiling model: " << file_path << std::endl;

                sigma::convert::assimp_converter imported{ file_path };
                for (auto mesh_name : imported.static_mesh_names()) {
                    sigma::resource::development_identifier rid("static_mesh", file_path, mesh_name);
                    auto final_path = outputdir / std::to_string(rid.value());

                    sigma::graphics::static_mesh_data mesh;
                    imported.convert_static_mesh(mesh_name, mesh);

                    std::ofstream stream(final_path.string(), std::ios::binary | std::ios::out);
                    boost::archive::binary_oarchive oa(stream);
                    oa << mesh;
                }

                auto scene_path = file_path.replace_extension(".scn");

                Json::Value scene;
                if (boost::filesystem::exists(scene_path)) {
                    std::ifstream in_scene(scene_path.string());
                    in_scene >> scene;
                }

                for (auto object_name : imported.scene_object_names())
                    imported.convert_object(object_name, scene[object_name]);

                scene_path = sigma::filesystem::make_relative(boost::filesystem::current_path(), scene_path);
                scene_path = outputdir / scene_path;
                std::ofstream outscene(scene_path.string());
                outscene << scene;
            } else {
                std::cerr << "file '" << file_path << "' does not exist!" << std::endl;
                return -1;
            }
        } else {
            std::cerr << "file '" << file_path << "' is not contained in '" << boost::filesystem::current_path() << "'!" << std::endl;
            return -1;
        }
    }

    return 0;
}
