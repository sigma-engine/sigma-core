#include <compiler_util.hpp>
#include <texture_compiler.hpp>

#include <sigma/graphics/texture.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <boost/gil/extension/io/tiff_dynamic_io.hpp>

#include <unordered_map>

namespace sigma {

enum class input_file_type : unsigned int {
    tiff,
    jpeg,
    png
};

static std::unordered_map<std::string, input_file_type> ext_to_type = {
    { ".tiff", input_file_type::tiff },
    { ".tif", input_file_type::tiff },
    { ".jpg", input_file_type::jpeg },
    { ".jpeg", input_file_type::jpeg },
    { ".jpe", input_file_type::jpeg },
    { ".jif", input_file_type::jpeg },
    { ".jfif", input_file_type::jpeg },
    { ".jfi", input_file_type::jpeg },
    { ".png", input_file_type::png },
};

bool is_texture(boost::filesystem::path file)
{
    return ext_to_type.count(file.extension().string()) > 0;
}

bool compile_textures(boost::filesystem::path outputdir, std::vector<boost::filesystem::path> textures)
{
    bool all_good = true;
    for (const auto& file_path : textures) {
        if (!resource_has_changes(outputdir, file_path))
            continue;

        std::cout << file_path.filename().string() << std::endl;

        boost::gil::rgba8_image_t image;

        try {
            std::string file_path_string = file_path.string();

            switch (ext_to_type[file_path.extension().string()]) {
            case input_file_type::tiff: {
                boost::gil::tiff_read_and_convert_image(file_path_string, image);
                break;
            }
            case input_file_type::jpeg: {
                boost::gil::jpeg_read_and_convert_image(file_path_string, image);
                break;
            }
            case input_file_type::png: {
                boost::gil::png_read_and_convert_image(file_path_string, image);
                break;
            }
            }

            sigma::resource::development_identifier rid("texture", file_path);
            auto final_path = outputdir / std::to_string(rid.value());

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

            std::ofstream stream{ final_path.string(), std::ios::binary };
            boost::archive::binary_oarchive oa(stream);
            oa << texture;

            touch_stamp_file(outputdir, file_path);
        } catch (...) { // TODO cacth the correct error types and get the messages
            all_good = false;
            std::cerr << "scompiler: error: could not compile texture '" << file_path << "'!\n";
        }
    }
    return all_good;
}
}
