#include "assimp_converter.hpp"

#include <fstream>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>

#include <sigma/graphics/static_mesh.hpp>
#include <sigma/util/filesystem.hpp>

namespace po = boost::program_options;

int main(int argc, char const* argv[])
{
    po::options_description global_options("Options");
    global_options.add_options()("help,h", "Show this help message")("output,o", po::value<boost::filesystem::path>()->default_value(boost::filesystem::current_path()), "output directory")("input-files", po::value<std::vector<boost::filesystem::path> >(), "input model files");

    po::positional_options_description positional_options;
    positional_options.add("input-files", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(global_options).positional(positional_options).run(), vm);

    if (vm.count("help")) {
        std::cout << global_options << std::endl;
        return -1;
    }

    if (vm.count("input-files") <= 0) {
        //std::cerr << "scmodel: fatal error: no input files." << std::endl;
        return 0;
    }

    auto outputdir = vm["output"].as<boost::filesystem::path>();
    boost::filesystem::create_directories(outputdir);

    for (auto file_path :
        vm["input-files"].as<std::vector<boost::filesystem::path> >()) {
        file_path = boost::filesystem::absolute(file_path);
        if (sigma::util::directory_contains_file(boost::filesystem::current_path(), file_path)) {
            if (boost::filesystem::exists(file_path)) {
                std::cout <<"Compiling model: " << file_path << std::endl;

                sigma::convert::assimp_converter imported{ file_path };
                for (auto mesh_name : imported.static_mesh_names()) {
                    sigma::resource::development_identifier rid("static_mesh", file_path, mesh_name);
                    auto final_path = outputdir / std::to_string(rid.value());

                    sigma::graphics::static_mesh mesh;
                    imported.convert_static_mesh(mesh_name, mesh);

                    std::ofstream stream(final_path.string(), std::ios::binary | std::ios::out);
                    boost::archive::binary_oarchive oa(stream);
                    oa << mesh;
                }
            } else {
                std::cerr << "file '" << file_path << "' does not exist!" << std::endl;
            }
        } else {
            std::cerr << "file '" << file_path << "' is not contained in '" << boost::filesystem::current_path() << "'!" << std::endl;
        }
    }

    return 0;
}
