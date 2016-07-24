#include <fstream>
#include <iostream>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <FreeImage.h>

#include <sigma/graphics/texture.hpp>
#include <sigma/resource/identifier.hpp>
#include <sigma/util/compile_time_hash.hpp>
#include <sigma/util/filesystem.hpp>

namespace po = boost::program_options;

int main(int argc, char const* argv[])
{
    po::options_description global_options("Options");
    global_options.add_options()("help,h", "Show this help message")("output,o", po::value<boost::filesystem::path>()->default_value(boost::filesystem::current_path()), "output directory")("input-files", po::value<std::vector<boost::filesystem::path> >(), "input textures files");

    po::positional_options_description positional_options;
    positional_options.add("input-files", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(global_options).positional(positional_options).run(), vm);

    if (vm.count("help")) {
        std::cout << global_options << std::endl;
        return -1;
    }

    if (vm.count("input-files") <= 0) {
        //std::cerr << "sctexture: fatal error: no input files." << std::endl;
        return 0;
    }

    FreeImage_Initialise(); // TODO use RAII

    auto outputdir = vm["output"].as<boost::filesystem::path>() / "opengl";
    boost::filesystem::create_directories(outputdir);

    for (auto file_path : vm["input-files"].as<std::vector<boost::filesystem::path> >()) {
        file_path = boost::filesystem::absolute(file_path);
        if (sigma::util::directory_contains_file(boost::filesystem::current_path(), file_path)) {
            if (boost::filesystem::exists(file_path)) {
                sigma::resource::development_identifier rid("texture", file_path);
                auto final_path = outputdir / std::to_string(rid.value());

                std::string file_path_string = file_path.string();
                FREE_IMAGE_FORMAT format = FreeImage_GetFileType(file_path_string.c_str()); //Automatocally detects the format(from over 20 formats!)
                FIBITMAP* image = FreeImage_Load(format, file_path_string.c_str());

                FIBITMAP* temp = image;
                image = FreeImage_ConvertTo32Bits(image);
                FreeImage_Unload(temp);

                auto w = FreeImage_GetWidth(image);
                auto h = FreeImage_GetHeight(image);

                std::vector<unsigned char> data(4 * w * h);
                char* pixels = (char*)FreeImage_GetBits(image); // TODO use FreeImage_ConvertToRawBits
                //FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).

                for (int j = 0; j < w * h; j++) {
                    data[j * 4 + 0] = pixels[j * 4 + 2];
                    data[j * 4 + 1] = pixels[j * 4 + 1];
                    data[j * 4 + 2] = pixels[j * 4 + 0];
                    data[j * 4 + 3] = pixels[j * 4 + 3];
                }

                sigma::graphics::texture texture;
                texture.set_data(w, h, data);

                std::ofstream stream{ final_path.string(), std::ios::binary | std::ios::out };
                boost::archive::binary_oarchive oa(stream);

                oa << texture;

                FreeImage_Unload(image);
            } else {
                std::cerr << "sctexture: error: file '" << file_path << "' does not exist!" << std::endl;
            }
        } else {
            std::cerr << "sctexture: error: file '" << file_path << "' is not contained in '" << boost::filesystem::current_path() << "'!" << std::endl;
        }
    }

    //FreeImage_DeInitialise();

    return 0;
}
