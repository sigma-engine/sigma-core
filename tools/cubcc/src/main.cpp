#include <sigma/graphics/cubemap.hpp>
#include <sigma/util/filesystem.hpp>

#include <json/json.h>

#include <boost/algorithm/string/case_conv.hpp>
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
    ("dependency,M", "List dependencies of the cubemap conversion.")
    ("source-file,c", boost::program_options::value<std::string>()->required(), "The cubemap file to convert.");
    // clang-format on

    positional.add("source-file", -1);

    try {
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(global).positional(positional).run(), vm);
        boost::program_options::notify(vm);

        auto source_file = boost::filesystem::canonical(vm["source-file"].as<std::string>());
        if (!boost::filesystem::exists(source_file))
            throw std::runtime_error("'" + source_file.string() + "' does not exist.");

        auto source_directory = boost::filesystem::canonical(vm["source-directory"].as<std::string>());
        auto build_directory = boost::filesystem::canonical(vm["build-directory"].as<std::string>());

        auto rid = "cubemap" / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
        auto output_file = build_directory / "data" / rid;

        std::ifstream file(source_file.string());
        Json::Value settings;
        file >> settings;

        std::vector<std::string> faces = { "right", "left", "top", "bottom", "back", "front" };
        for (auto face : faces) {
            if (!settings.isMember(face))
                throw std::runtime_error("missing " + face + " cubemap face.");
        }

        if (vm.count("dependency")) {
            boost::filesystem::path dependency_path = output_file;
            dependency_path.replace_extension(source_file.extension().string() + ".dependency");
            std::ofstream dep{ dependency_path.string() };

            std::regex re{ "[^a-zA-Z0-9]" };
            dep << "set(" << std::regex_replace(rid.string(), re, "_") << "_DEPENDS\n";
            for (auto face : faces)
                dep << (build_directory / "data" / "texture" / settings[face].asString()) << '\n';
            dep << ")\n";

            return 0;
        }

        sigma::graphics::cubemap cubemap;
        cubemap.right = boost::filesystem::path{ "texture" } / settings["right"].asString();
        cubemap.left = boost::filesystem::path{ "texture" } / settings["left"].asString();
        cubemap.top = boost::filesystem::path{ "texture" } / settings["top"].asString();
        cubemap.bottom = boost::filesystem::path{ "texture" } / settings["bottom"].asString();
        cubemap.back = boost::filesystem::path{ "texture" } / settings["back"].asString();
        cubemap.front = boost::filesystem::path{ "texture" } / settings["front"].asString();

        auto output_folder = output_file.parent_path();
        if (!boost::filesystem::exists(output_folder))
            boost::filesystem::create_directories(output_folder);

        std::ofstream stream{ output_file.string(), std::ios::binary };
        boost::archive::binary_oarchive oa(stream);
        oa << cubemap;
    } catch (boost::program_options::error& e) {
        std::cerr << "cubcc\nerror: " << e.what() << '\n';
        std::cerr << global << '\n';
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "cubecc\nerror: " << e.what() << '\n';
        return -1;
    }
    return 0;
}
