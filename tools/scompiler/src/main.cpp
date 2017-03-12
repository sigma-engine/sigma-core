#include <cubemap_compiler.hpp>
#include <material_compiler.hpp>
#include <model_compiler.hpp>
#include <post_process_effect_compiler.hpp>
#include <shader_compiler.hpp>
#include <texture_compiler.hpp>

#include <sigma/util/filesystem.hpp>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <algorithm>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char const* argv[])
{
    po::options_description global_options("Options");
    // clang-format off
    global_options.add_options()("help,h", "Show this help message")
    ("output,o", po::value<std::string>()->default_value((boost::filesystem::current_path()/"data").string()), "output directory")
	("include-dir,I", po::value<std::vector<std::string>>()->default_value(std::vector<std::string>{}, "")->implicit_value(std::vector<std::string>{},"."), "Include directory")
    ("input-files", po::value<std::vector<std::string>>(), "input resource files");
    // clang-format on

    po::positional_options_description positional_options;
    positional_options.add("input-files", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(global_options).positional(positional_options).run(), vm);

    if (vm.count("help")) {
        std::cout << global_options << '\n';
        return 0;
    }

    if (vm.count("input-files") <= 0) {
        std::cerr << "scompiler: fatal error: no input files.\n";
        return -1;
    }

    boost::filesystem::path outputdir{ vm["output"].as<std::string>() };
    boost::filesystem::create_directories(outputdir);

    std::vector<boost::filesystem::path> include_dirs;
    include_dirs.push_back(boost::filesystem::current_path());

    auto string_dirs = vm["include-dir"].as<std::vector<std::string>>();
    std::copy(string_dirs.cbegin(), string_dirs.cend(), std::back_inserter(include_dirs));

    std::vector<boost::filesystem::path> textures;
    std::vector<boost::filesystem::path> cubemaps;
    std::vector<boost::filesystem::path> shaders;
    std::vector<boost::filesystem::path> materials;
    std::vector<boost::filesystem::path> models;
    std::vector<boost::filesystem::path> post_process_effects;
    for (auto file_path_string : vm["input-files"].as<std::vector<std::string>>()) {
        boost::filesystem::path file_path{ file_path_string };
        file_path = boost::filesystem::absolute(file_path);
        if (sigma::filesystem::contains_file(boost::filesystem::current_path(), file_path) && boost::filesystem::exists(file_path)) {
            if (sigma::is_texture(file_path))
                textures.push_back(file_path);
            else if (sigma::is_cubemap(file_path))
                cubemaps.push_back(file_path);
            else if (sigma::is_shader(file_path))
                shaders.push_back(file_path);
            else if (sigma::is_material(file_path))
                materials.push_back(file_path);
            else if (sigma::is_model(file_path))
                models.push_back(file_path);
            else if (sigma::is_post_process_effect(file_path))
                post_process_effects.push_back(file_path);
        } else {
            std::cerr << "scompiler: error: file '" << file_path << "' is not contained in '" << boost::filesystem::current_path() << "'!\n";
            return -1;
        }
    }

    if (!sigma::compile_textures(outputdir, textures))
        return -1;

    if (!sigma::compile_cubemaps(outputdir, cubemaps))
        return -1;

    if (!sigma::compile_shaders(outputdir, include_dirs, shaders))
        return -1;

    if (!sigma::compile_materials(outputdir, materials))
        return -1;

    if (!sigma::compile_models(outputdir, models))
        return -1;

    if (!sigma::compile_post_process_effects(outputdir, post_process_effects))
        return -1;

    return 0;
}
