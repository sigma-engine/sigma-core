#include <hdr_io.hpp>

#include <sigma/graphics/texture.hpp>
#include <sigma/util/filesystem.hpp>

#include <json/json.h>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

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

template <class Image>
void copy_texture(const Image& image, sigma::graphics::texture& texture)
{
    auto view = boost::gil::const_view(image);
    using pixel = typename decltype(view)::value_type;
    texture.data.resize(image.width() * image.height() * sizeof(pixel));
    texture.size = glm::ivec2{ image.width(), image.height() };
    boost::gil::copy_pixels(view, boost::gil::interleaved_view(view.width(), view.height(), (pixel*)texture.data.data(), view.width() * sizeof(pixel)));
}

template <class Image>
void compile_texture(const boost::filesystem::path& file_path, sigma::graphics::texture& texture)
{
    auto file_path_string = file_path.string();
    auto extension = boost::to_lower_copy(file_path.extension().string());
    auto it = ext_to_type.find(extension);
    if (it == ext_to_type.end())
        throw std::runtime_error("Unknown file format with extention '" + extension + "'");

    Image image;
    switch (it->second) {
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
        hdr_read_and_convert_image(file_path_string, image);
        break;
    }
    }

    copy_texture(image, texture);
}

int main(int argc, char const* argv[])
{
    boost::program_options::options_description global{ "options" };
    boost::program_options::positional_options_description positional;

    // clang-format off
    global.add_options()
    ("source-directory,H", boost::program_options::value<std::string>()->default_value(boost::filesystem::current_path().string()), "The top level package directory.")
    ("build-directory,B", boost::program_options::value<std::string>()->default_value(boost::filesystem::current_path().string()), "The top level project build directory.")
    ("output,l", "List outputs of the texture conversion.")
    ("dependency,M", "List dependencies of the texture conversion.")
    ("source-file,c", boost::program_options::value<std::string>()->required(), "The texture file to convert.");
    // clang-format on

    positional.add("source-file", -1);

    try {
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(global).positional(positional).run(), vm);
        boost::program_options::notify(vm);

        auto source_file = boost::filesystem::canonical(vm["source-file"].as<std::string>());
        auto settings_path = source_file.parent_path() / (source_file.stem().string() + ".stex");
        if (!boost::filesystem::exists(source_file))
            throw std::runtime_error("'" + source_file.string() + "' does not exist.");

        auto source_directory = boost::filesystem::canonical(vm["source-directory"].as<std::string>());
        auto build_directory = boost::filesystem::canonical(vm["build-directory"].as<std::string>());

        auto rid = "texture" / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
        auto output_file = build_directory / "data" / rid;

        if (vm.count("output")) {
            std::cout << output_file.string() << '\n';
            return 0;
        } else if (vm.count("dependency")) {
            boost::filesystem::path dependency_path = output_file;
            dependency_path.replace_extension(source_file.extension().string() + ".deps");
            std::ofstream dep{ dependency_path.string() };

            std::regex re{ "[^a-zA-Z0-9]" };
            dep << "set(" << std::regex_replace(rid.string(), re, "_") << "_deps\n";
            if (boost::filesystem::exists(settings_path))
                dep << settings_path << "\n";
            dep << ")\n";
            return 0;
        }

        Json::Value settings(Json::objectValue);
        settings["format"] = "rgb8";
        settings["filter"]["minification"] = "linear";
        settings["filter"]["magnification"] = "linear";
        settings["filter"]["mipmap"] = "linear";

        if (boost::filesystem::exists(settings_path)) {
            std::ifstream file(settings_path.string());
            file >> settings;
        }

        sigma::graphics::texture texture;

        sigma::json::from_json(settings["filter"]["minification"], texture.minification_filter);
        sigma::json::from_json(settings["filter"]["magnification"], texture.magnification_filter);
        sigma::json::from_json(settings["filter"]["mipmap"], texture.mipmap_filter);
        sigma::json::from_json(settings["format"], texture.format);

        switch (texture.format) {
        case sigma::graphics::texture_format::RGB8: {
            compile_texture<boost::gil::rgb8_image_t>(source_file, texture);
            break;
        }
        case sigma::graphics::texture_format::RGBA8: {
            compile_texture<boost::gil::rgba8_image_t>(source_file, texture);
            break;
        }
        case sigma::graphics::texture_format::RGB32F: {
            compile_texture<boost::gil::rgb32f_image_t>(source_file, texture);
            break;
        }
        }

        auto output_folder = output_file.parent_path();
        if (!boost::filesystem::exists(output_folder))
            boost::filesystem::create_directories(output_folder);

        std::ofstream stream{ output_file.string(), std::ios::binary };
        boost::archive::binary_oarchive oa(stream);
        oa << texture;
    } catch (boost::program_options::error& e) {
        std::cerr << "texcc\nerror: " << e.what() << '\n';
        std::cerr << global << '\n';
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "texcc\nerror: " << e.what() << '\n';
        return -1;
    }
    return 0;
}
