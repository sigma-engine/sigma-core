#include <material_compile.hpp>
#include <post_process_effect_compile.hpp>
#include <shader_compile.hpp>

#include <sigma/graphics/material.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/resource/database.hpp>
#include <sigma/util/filesystem.hpp>

#include <json/json.h>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>

int main(int argc, char const* argv[])
{
    glslang::process glslang_proc;

    boost::program_options::options_description global{ "options" };
    boost::program_options::positional_options_description positional;

    // clang-format off
    global.add_options()
    ("source-directory,H", boost::program_options::value<std::string>()->default_value(boost::filesystem::current_path().string()), "The top level package directory.")
    ("build-directory,B", boost::program_options::value<std::string>()->default_value(boost::filesystem::current_path().string()), "The top level project build directory.")
    ("include-directory,I", boost::program_options::value<std::vector<std::string>>()->default_value(std::vector<std::string>{}, ""), "Extra include directories")
    ("source-file,c", boost::program_options::value<std::string>()->required(), "The shader file to convert.");
    // clang-format on

    positional.add("source-file", -1);

    try {
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(global).positional(positional).run(), vm);
        boost::program_options::notify(vm);

        auto source_directory = boost::filesystem::canonical(vm["source-directory"].as<std::string>());
        auto build_directory = boost::filesystem::canonical(vm["build-directory"].as<std::string>());
        auto data_directory = build_directory / "data";

        sigma::resource::database<sigma::graphics::texture> texture_database{ data_directory, "texture" };
        sigma::resource::database<sigma::graphics::cubemap> cubemap_database{ data_directory, "cubemap" };
        sigma::resource::database<sigma::graphics::shader> shader_database{ data_directory, "shader" };
        sigma::resource::database<sigma::graphics::technique> technique_database{ data_directory, "technique" };
        sigma::resource::database<sigma::graphics::material> material_database{ data_directory, "material" };
        sigma::resource::database<sigma::graphics::static_mesh> static_mesh_database{ data_directory, "static_mesh" };
        sigma::resource::database<sigma::graphics::post_process_effect> post_process_effect_database{ data_directory, "post_process_effect" };

        std::vector<boost::filesystem::path> include_directories;
        for (auto path : vm["include-directory"].as<std::vector<std::string>>()) {
            boost::trim(path);
            if (path.size() > 0) {
                include_directories.push_back(path);
            }
        }

        auto source_file = boost::filesystem::canonical(vm["source-file"].as<std::string>());
        if (!boost::filesystem::exists(source_file))
            throw std::runtime_error("The file '" + source_file.string() + "' does not exist.");

        auto rid = sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
        std::vector<boost::filesystem::path> dependencies;

        auto extension = boost::to_lower_copy(source_file.extension().string());
        if (extension == ".stech") {
            rid = "technique" / rid;

            std::ifstream file(source_file.string());
            Json::Value settings;
            file >> settings;

            std::vector<boost::filesystem::path> technique_cid;
            sigma::graphics::technique technique;
            std::tie(technique_cid, technique) = compile_technique(shader_database, settings);

            technique_database.insert(technique_cid, technique);
        } else if (extension == ".smat") {
            rid = "material" / rid;

            std::ifstream file(source_file.string());
            Json::Value settings;
            file >> settings;

            auto material = compile_material(texture_database, cubemap_database, shader_database, technique_database, settings);
            material_database.insert({ rid }, material);
        } else if (extension == ".eff") {
            rid = "post_process_effect" / rid;

            std::ifstream file(source_file.string());
            Json::Value settings;
            file >> settings;

            auto post_process_effect = compile_post_process_effect(texture_database, cubemap_database, shader_database, technique_database, static_mesh_database, settings);
            post_process_effect_database.insert({ rid }, post_process_effect);
        } else {
            const static std::unordered_map<std::string, std::pair<sigma::graphics::shader_type, std::string>> extention_type_map = {
                { ".vert", { sigma::graphics::shader_type::vertex, "vertex" } },
                { ".tesc", { sigma::graphics::shader_type::geometry, "tessellation_control" } },
                { ".tese", { sigma::graphics::shader_type::geometry, "tessellation_evaluation" } },
                { ".geom", { sigma::graphics::shader_type::geometry, "geometry" } },
                { ".frag", { sigma::graphics::shader_type::fragment, "fragment" } },
                { ".glsl", { sigma::graphics::shader_type::header, "header" } }
            };

            auto shader_type_info = extention_type_map.find(extension);
            if (shader_type_info == extention_type_map.end())
                throw std::runtime_error("Unknown shader type with extention '" + extension + "'");

            rid = shader_type_info->second.second / rid;

            std::ifstream source{ source_file.string() };
            auto shader = compile_shader(dependencies, include_directories, shader_type_info->second.first, source_file, std::string{ std::istreambuf_iterator<char>{ source.rdbuf() }, std::istreambuf_iterator<char>{} });

            shader_database.insert({ rid }, shader);
        }

        // Output dependency file
        {
            auto dependency_path = (data_directory / rid).replace_extension(source_file.extension().string() + ".dependency");
            if (!boost::filesystem::exists(dependency_path.parent_path()))
                boost::filesystem::create_directories(dependency_path.parent_path());
            std::ofstream dep{ dependency_path.string() };

            std::regex re{ "[^a-zA-Z0-9]" };
            dep << "set(" << std::regex_replace(rid.string(), re, "_") << "_DEPENDS\n";
            for (const auto& dependency : dependencies)
                dep << dependency << '\n';
            dep << ")\n";
        }
    } catch (const boost::program_options::error& e) {
        std::cerr << "shdrcc\nerror: " << e.what() << '\n';
        std::cerr << global << '\n';
        return -1;
    } catch (const boost::wave::cpp_exception& e) {
        std::cerr << e.file_name() << "(" << e.line_no() << "): " << e.description() << '\n';
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "shdrcc\nerror: " << e.what() << '\n';
        return -1;
    }
    return 0;
}
