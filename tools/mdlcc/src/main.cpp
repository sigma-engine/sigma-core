#include <assimp_converter.hpp>

#include <sigma/graphics/static_mesh.hpp>
#include <sigma/util/filesystem.hpp>

#include <json/json.h>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

int main(int argc, char const* argv[])
{
    boost::program_options::options_description global{ "options" };
    boost::program_options::positional_options_description positional;

    // clang-format off
    global.add_options()
    ("source-directory,H", boost::program_options::value<std::string>()->default_value(boost::filesystem::current_path().string()), "The top level package directory.")
    ("build-directory,B", boost::program_options::value<std::string>()->default_value(boost::filesystem::current_path().string()), "The top level project build directory.")
    ("dependency,M", "List dependencies of the static mesh conversion.")
    ("source-file,c", boost::program_options::value<std::string>()->required(), "The static mesh file to convert.");
    // clang-format on

    positional.add("source-file", -1);

    try {
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(global).positional(positional).run(), vm);
        boost::program_options::notify(vm);

        auto source_file = boost::filesystem::canonical(vm["source-file"].as<std::string>());
        auto settings_path = source_file.parent_path() / (source_file.stem().string() + ".scn");
        if (!boost::filesystem::exists(source_file))
            throw std::runtime_error("'" + source_file.string() + "' does not exist.");

        auto source_directory = boost::filesystem::canonical(vm["source-directory"].as<std::string>());
        auto build_directory = boost::filesystem::canonical(vm["build-directory"].as<std::string>());

        auto package_path = sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
        auto rid = "blueprint" / package_path;
        auto output_file = build_directory / "data" / rid;

        if (vm.count("dependency")) {
            boost::filesystem::path dependency_path = output_file;
            dependency_path.replace_extension(source_file.extension().string() + ".dependency");
            std::ofstream dep{ dependency_path.string() };

            std::regex re{ "[^a-zA-Z0-9]" };
            dep << "set(" << std::regex_replace(rid.string(), re, "_") << "_DEPENDS\n";
            if (boost::filesystem::exists(settings_path))
                dep << settings_path << "\n";
            dep << ")\n";
            return 0;
        }

        sigma::assimp_converter imported{ source_directory, source_file };

        for (auto mesh_name : imported.static_mesh_names()) {
            if (!boost::algorithm::ends_with(mesh_name, "_high")) {
                auto mesh_rid = build_directory / "data" / "static_mesh" / package_path / mesh_name;

                sigma::graphics::static_mesh mesh;
                imported.convert_static_mesh(mesh_name, mesh);

                auto output_folder = mesh_rid.parent_path();
                if (!boost::filesystem::exists(output_folder))
                    boost::filesystem::create_directories(output_folder);

                std::ofstream stream(mesh_rid.string(), std::ios::binary | std::ios::out);
                boost::archive::binary_oarchive oa(stream);
                oa << mesh;
            }
        }

        Json::Value settings(Json::objectValue);
        if (boost::filesystem::exists(settings_path)) {
            std::ifstream file(settings_path.string());
            file >> settings;
        }

        for (auto object_name : imported.scene_object_names()) {
            if (!boost::algorithm::ends_with(object_name, "_high")) {
                imported.convert_object(object_name, settings[object_name]);
            }
        }

        auto output_folder = output_file.parent_path();
        if (!boost::filesystem::exists(output_folder))
            boost::filesystem::create_directories(output_folder);

        std::ofstream stream{ output_file.string(), std::ios::binary };
        stream << settings;
    } catch (boost::program_options::error& e) {
        std::cerr << "mdlcc\nerror: " << e.what() << '\n';
        std::cerr << global << '\n';
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "mdlcc\nerror: " << e.what() << '\n';
        return -1;
    }
    return 0;
}
