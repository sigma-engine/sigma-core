#include <fstream>
#include <iostream>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/program_options.hpp>

#include <json/json.h>

#include <sigmafive/graphics/material.hpp>
#include <sigmafive/util/compile_time_hash.hpp>
#include <sigmafive/util/filesystem.hpp>

namespace po = boost::program_options;

int main(int argc, char const* argv[])
{
    po::options_description global_options("Options");
    global_options.add_options()("help,h", "Show this help message")("output,o", po::value<boost::filesystem::path>()->default_value(boost::filesystem::current_path()), "output directory")("input-files", po::value<std::vector<boost::filesystem::path> >(), "input shaders files");

    po::positional_options_description positional_options;
    positional_options.add("input-files", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(global_options).positional(positional_options).run(), vm);

    if (vm.count("help")) {
        std::cout << global_options << std::endl;
        return -1;
    }

    if (vm.count("input-files") <= 0) {
        std::cerr << "shader-compiler: fatal error: no input files." << std::endl;
        return -1;
    }

    auto outputdir = vm["output"].as<boost::filesystem::path>();
    boost::filesystem::create_directories(outputdir);

    for (auto file_path :
        vm["input-files"].as<std::vector<boost::filesystem::path> >()) {
        file_path = boost::filesystem::absolute(file_path);
        if (sigmafive::util::directory_contains_file(boost::filesystem::current_path(), file_path)) {
            if (boost::filesystem::exists(file_path)) {
                sigmafive::resource::development_identifier rid("material", file_path);
                auto final_path = outputdir / std::to_string(rid.value());

                std::ifstream file{ file_path.string(), std::ios::in };
                Json::Value material_data;
                file >> material_data;

                sigmafive::graphics::material material;
                for (auto it = material_data.begin(); it != material_data.end(); ++it) {
                    const auto& value = *it;
                    if (it.key() == "vertex") {
                        auto vertex_shader = value.asString();
                        if (!boost::starts_with(vertex_shader, "vertex://"))
                            vertex_shader = "vertex://" + vertex_shader;
                        material.vertex_shader = vertex_shader;
                    } else if (it.key() == "fragment") {
                        auto fragment_shader = value.asString();
                        if (!boost::starts_with(fragment_shader, "fragment://"))
                            fragment_shader = "fragment://" + fragment_shader;
                        material.fragment_shader = fragment_shader;
                    } else if (it.key() == "textures") {
                        for (const auto& texture_object : value) {
                            auto texture_source = texture_object["source"].asString();
                            if (!boost::starts_with(texture_source, "texture://"))
                                texture_source = "texture://" + texture_source;
                            material.textures.emplace_back(texture_object["name"].asString(), texture_source);
                        }
                    }
                }

                std::ofstream stream{ final_path.string(), std::ios::binary | std::ios::out };
                boost::archive::binary_oarchive oa(stream);
                oa << material;
            } else {
                std::cerr << "file '" << file_path << "' does not exist!" << std::endl;
            }
        } else {
            std::cerr << "file '" << file_path << "' is not contained in '" << boost::filesystem::current_path() << "'!" << std::endl;
        }
    }

    return 0;
}
