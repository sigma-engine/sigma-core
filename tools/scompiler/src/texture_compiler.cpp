#include <compiler_util.hpp>
#include <texture_compiler.hpp>

#include <sigma/graphics/texture.hpp>
#include <sigma/util/json_conversion.hpp>

#include <json/json.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>

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
        Json::Value import_settings(Json::objectValue);
        import_settings["format"] = "rgba8";
        import_settings["filter"]["minification"] = "linear";
        import_settings["filter"]["magnification"] = "linear";
        import_settings["filter"]["mipmap"] = "linear";

        boost::filesystem::path import_settings_path = file_path.parent_path() / (file_path.stem().string() + ".stex");
        if (boost::filesystem::exists(import_settings_path)) {
            std::ifstream file(import_settings_path.string());
            file >> import_settings;
        } else {
            std::ofstream file(import_settings_path.string());
            file << import_settings;
        }

        if (!resource_has_changes(outputdir, file_path))
            continue;

        std::cout << file_path.filename().string() << std::endl;

        try {
            boost::gil::rgba8_image_t image;

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

            sigma::resource::identifier rid("texture", file_path);
            auto final_path = outputdir / std::to_string(rid.value());

            sigma::graphics::texture_data texture;
            texture.size = glm::ivec2{ image.width(), image.height() };

            if (!json::from_json(import_settings["filter"]["minification"], texture.minification_filter) || texture.minification_filter == graphics::texture_filter::NONE)
                throw std::runtime_error("filter.minification='" + import_settings["filter"]["minification"].asString() + "'");

            if (!json::from_json(import_settings["filter"]["magnification"], texture.magnification_filter) || texture.magnification_filter == graphics::texture_filter::NONE)
                throw std::runtime_error("filter.magnification='" + import_settings["filter"]["magnification"].asString() + "'");

            if (!json::from_json(import_settings["filter"]["mipmap"], texture.mipmap_filter))
                throw std::runtime_error("filter.mipmap='" + import_settings["filter"]["mipmap"].asString() + "'");

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

            touch_stamp_file(outputdir, file_path, { import_settings_path });
        } catch (...) { // TODO cacth the correct error types and get the messages
            all_good = false;
            std::cerr << "scompiler: error: could not compile texture '" << file_path << "'!\n";
        }
    }
    return all_good;
}
}
