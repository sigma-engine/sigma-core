#include "../tools/effcc/include/shader_technique_compiler.hpp"

#include <sigma/graphics/material.hpp>
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
    ("dependency,M", "List dependencies of the material conversion.")
    ("source-file,c", boost::program_options::value<std::string>()->required(), "The material file to convert.");
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

        auto rid = "material" / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
        auto output_file = build_directory / "data" / rid;

        std::ifstream file(source_file.string());
        Json::Value settings;
        file >> settings;

        if (vm.count("dependency")) {
            boost::filesystem::path dependency_path = output_file;
            dependency_path.replace_extension(source_file.extension().string() + ".dependency");
            std::ofstream dep{ dependency_path.string() };

            std::regex re{ "[^a-zA-Z0-9]" };
            dep << "set(" << std::regex_replace(rid.string(), re, "_") << "_DEPENDS\n";
            for (auto stage : { "vertex", "fragment", "geometry" }) {
                if (settings.isMember(stage))
                    dep << (build_directory / "data" / stage / settings[stage].asString()) << '\n';
            }
            dep << ")\n";

            return 0;
        }

        sigma::graphics::material material;
        sigma::compile_shader_technique(material, settings);

        if (settings.isMember("transparent"))
            material.transparent = settings["transparent"].asBool();

        auto output_folder = output_file.parent_path();
        if (!boost::filesystem::exists(output_folder))
            boost::filesystem::create_directories(output_folder);

        std::ofstream stream{ output_file.string(), std::ios::binary };
        boost::archive::binary_oarchive oa(stream);
        oa << material;
    } catch (boost::program_options::error& e) {
        std::cerr << "mtlcc\nerror: " << e.what() << '\n';
        std::cerr << global << '\n';
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "mtlcc\nerror: " << e.what() << '\n';
        return -1;
    }
    return 0;
}
