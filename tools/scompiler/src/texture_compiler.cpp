#include <compiler_util.hpp>
#include <hdr_io.hpp>
#include <texture_compiler.hpp>

#include <sigma/graphics/texture.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/json_conversion.hpp>

#include <json/json.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>

#include <iostream>
#include <unordered_map>

namespace sigma {

enum class input_file_type : unsigned int {
    tiff,
    jpeg,
    png,
    hdr
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
    { ".hdr", input_file_type::hdr }
};

bool is_texture(boost::filesystem::path file)
{
    return ext_to_type.count(file.extension().string()) > 0;
}

template <class Img>
void copy_texture(const Img& image, graphics::texture& texture)
{
    auto view = boost::gil::const_view(image);
    using pixel = typename decltype(view)::value_type;
    texture.data.resize(image.width() * image.height() * sizeof(pixel));
    texture.size = glm::ivec2{ image.width(), image.height() };
    boost::gil::copy_pixels(view, boost::gil::interleaved_view(view.width(), view.height(), (pixel*)texture.data.data(), view.width() * sizeof(pixel)));
}

template <class Img>
void compile_texture(const boost::filesystem::path& file_path, graphics::texture& texture)
{
    std::string file_path_string = file_path.string();
    Img image;
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
    case input_file_type::hdr: {
        sigma::hdr_read_and_convert_image(file_path_string, image);
        break;
    }
    }

    copy_texture(image, texture);
}

bool compile_textures(boost::filesystem::path outputdir, boost::filesystem::path sourcedir, std::vector<boost::filesystem::path> textures)
{
    bool all_good = true;
    for (const auto& file_path : textures) {
        Json::Value import_settings(Json::objectValue);
        import_settings["format"] = "rgb8";
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

        try {
            auto rid = boost::filesystem::path{ "texture" } / sigma::filesystem::make_relative(sourcedir, file_path).replace_extension("");
            auto final_path = outputdir / rid;

            sigma::graphics::texture texture;

            json::from_json(import_settings["filter"]["minification"], texture.minification_filter);
            json::from_json(import_settings["filter"]["magnification"], texture.magnification_filter);
            json::from_json(import_settings["filter"]["mipmap"], texture.mipmap_filter);
            json::from_json(import_settings["format"], texture.format);

            switch (texture.format) {
            case graphics::texture_format::RGB8: {
                compile_texture<boost::gil::rgb8_image_t>(file_path, texture);
                break;
            }
            case graphics::texture_format::RGBA8: {
                compile_texture<boost::gil::rgba8_image_t>(file_path, texture);
                break;
            }
            case graphics::texture_format::RGB32F: {
                compile_texture<boost::gil::rgb32f_image_t>(file_path, texture);
                break;
            }
            }

            auto output_folder = final_path.parent_path();
            if (!boost::filesystem::exists(output_folder))
                boost::filesystem::create_directories(output_folder);

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
