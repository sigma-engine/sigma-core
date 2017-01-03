#include <sigma/graphics/texture.hpp>
#include <sigma/resource/identifier.hpp>
#include <sigma/util/compile_time_hash.hpp>
#include <sigma/util/filesystem.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem.hpp>
#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <boost/gil/extension/io/tiff_dynamic_io.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>

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

    auto outputdir = vm["output"].as<boost::filesystem::path>();
    boost::filesystem::create_directories(outputdir);

    for (auto file_path : vm["input-files"].as<std::vector<boost::filesystem::path> >()) {
        file_path = boost::filesystem::absolute(file_path);
        if (sigma::filesystem::contains_file(boost::filesystem::current_path(), file_path)) {
            if (boost::filesystem::exists(file_path)) {
                std::cout << "Compiling texture: " << file_path << std::endl;

                sigma::resource::development_identifier rid("texture", file_path);
                auto final_path = outputdir / std::to_string(rid.value());

                std::string file_path_string = file_path.string();
                boost::gil::rgba8_image_t image;

                auto ext = file_path.extension();
                if (ext == ".tiff" || ext == ".tif") {
                    boost::gil::tiff_read_and_convert_image(file_path_string, image);
                } else if (ext == ".jpg" || ext == ".jpeg" || ext == ".jpe" || ext == ".jif" || ext == ".jfif" || ext == ".jfi") {
                    boost::gil::jpeg_read_and_convert_image(file_path_string, image);
                } else if (ext == ".png") {
                    boost::gil::png_read_and_convert_image(file_path_string, image);
                } else {
                    std::cerr << "sctexture: error: input file \'" << file_path << "\' is not supported." << std::endl;
                    break;
                }

                sigma::graphics::texture_data texture;
                texture.size = glm::ivec2{ image.width(), image.height() };
                texture.pixels.reserve(image.width() * image.height() * 4);

                auto f = [&texture](boost::gil::rgba8_pixel_t p) {
                    texture.pixels.push_back(boost::gil::at_c<0>(p));
                    texture.pixels.push_back(boost::gil::at_c<1>(p));
                    texture.pixels.push_back(boost::gil::at_c<2>(p));
                    texture.pixels.push_back(boost::gil::at_c<3>(p));
                };

                boost::gil::for_each_pixel(boost::gil::const_view(image), std::function<void(boost::gil::rgba8_pixel_t)>(f));

                std::ofstream stream{ final_path.string(), std::ios::binary | std::ios::out };
                boost::archive::binary_oarchive oa(stream);

                oa << texture;

            } else {
                std::cerr << "sctexture: error: file '" << file_path << "' does not exist!" << std::endl;
            }
        } else {
            std::cerr << "sctexture: error: file '" << file_path << "' is not contained in '" << boost::filesystem::current_path() << "'!" << std::endl;
        }
    }

    return 0;
}
