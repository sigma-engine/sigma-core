#include <sigma/graphics/material.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/util/compile_time_hash.hpp>
#include <sigma/util/filesystem.hpp>

#include <json/json.h>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>
#include <string>

namespace po = boost::program_options;

template <class T>
void compile_technique(T& technique, const Json::Value& technique_data)
{
    for (auto it = technique_data.begin(); it != technique_data.end(); ++it) {
        const auto& value = *it;
        if (it.key() == "vertex") {
            technique.shaders[sigma::graphics::shader_type::vertex] = sigma::resource::identifier{ "vertex", value.asString() }; // TODO warn if tring to set shader more that once
        } else if (it.key() == "fragment") {
            technique.shaders[sigma::graphics::shader_type::fragment] = sigma::resource::identifier{ "fragment", value.asString() }; // TODO warn if tring to set shader more that once
        } else if (it.key() == "geometry") {
            technique.shaders[sigma::graphics::shader_type::geometry] = sigma::resource::identifier{ "geometry", value.asString() }; // TODO warn if tring to set shader more that once
        } else if (it.key() == "textures") {
            const auto& texture_object = *it;
            for (auto it2 = texture_object.begin(); it2 != texture_object.end(); ++it2)
                technique.textures[it2.key().asString()] = sigma::resource::identifier{ "texture", (*it2).asString() }; // TODO warn if tring to set texture more than once
        }
    }

    // TODO check for errors like no vertex or fragment shader
}

int main(int argc, char const* argv[])
{
    po::options_description global_options("Options");
    // clang-format off
    global_options.add_options()
    ("help,h", "Show this help message")
    ("output,o", po::value<boost::filesystem::path>()->default_value(boost::filesystem::current_path()), "output directory")
    ("input-files", po::value<std::vector<boost::filesystem::path> >(), "input shaders files");
    // clang-format on

    po::positional_options_description positional_options;
    positional_options.add("input-files", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(global_options).positional(positional_options).run(), vm);

    if (vm.count("help")) {
        std::cout << global_options << std::endl;
        return -1;
    }

    if (vm.count("input-files") <= 0) {
        std::cerr << "scmaterial: fatal error: no input files." << std::endl;
        return -1;
    }

    auto outputdir = vm["output"].as<boost::filesystem::path>();
    boost::filesystem::create_directories(outputdir);

    for (auto file_path : vm["input-files"].as<std::vector<boost::filesystem::path>>()) {
        file_path = boost::filesystem::absolute(file_path);
        if (sigma::filesystem::contains_file(boost::filesystem::current_path(), file_path)) {
            if (boost::filesystem::exists(file_path)) {
                std::ifstream file{ file_path.string(), std::ios::in };
                Json::Value technique_data;
                file >> technique_data;

                if (file_path.extension() == ".mat") {
                    std::cout << "Compiling material: " << file_path << std::endl;
                    sigma::graphics::material_data material;
                    compile_technique(material, technique_data);

                    if (technique_data.isMember("transparent"))
                        material.transparent = technique_data["transparent"].asBool();

                    // TODO check for errors like no vertex or fragment shader

                    sigma::resource::development_identifier rid("material", file_path);
                    auto final_path = outputdir / std::to_string(rid.value());
                    std::ofstream stream{ final_path.string(), std::ios::binary | std::ios::out };
                    boost::archive::binary_oarchive oa(stream);
                    oa << material;
                } else if (file_path.extension() == ".eff") {
                    std::cout << "Compiling post process effect: " << file_path << std::endl;
                    sigma::graphics::post_process_effect_data effect;
                    compile_technique(effect, technique_data);
                    if (technique_data.isMember("static_mesh"))
                        effect.mesh = sigma::resource::identifier{ "static_mesh", technique_data["static_mesh"].asString() };

                    sigma::resource::development_identifier rid("post_process_effect", file_path);
                    auto final_path = outputdir / std::to_string(rid.value());
                    std::ofstream stream{ final_path.string(), std::ios::binary | std::ios::out };
                    boost::archive::binary_oarchive oa(stream);
                    oa << effect;
                }
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
