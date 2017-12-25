#include <package_texture.hpp>

#include <hdr_io.hpp>

#include <sigma/util/filesystem.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>

#include <string>
#include <unordered_map>

enum class texture_source_type : unsigned int {
    tiff,
    jpeg,
    png,
    hdr
};

template <class Image>
void convert_texture(
    const boost::filesystem::path& source_file,
    texture_source_type source_type,
    sigma::graphics::texture& texture)
{
    auto file_path_string = source_file.string();

    Image image;
    switch (source_type) {
    case texture_source_type::tiff: {
        boost::gil::tiff_read_and_convert_image(file_path_string, image);
        break;
    }
    case texture_source_type::jpeg: {
        boost::gil::jpeg_read_and_convert_image(file_path_string, image);
        break;
    }
    case texture_source_type::png: {
        boost::gil::png_read_and_convert_image(file_path_string, image);
        break;
    }
    case texture_source_type::hdr: {
        hdr_read_and_convert_image(file_path_string, image);
        break;
    }
    }

    auto view = boost::gil::const_view(image);
    using pixel = typename decltype(view)::value_type;
    texture.data.resize(image.width() * image.height() * sizeof(pixel));
    texture.size = glm::ivec2{ image.width(), image.height() };
    boost::gil::copy_pixels(view, boost::gil::interleaved_view(view.width(), view.height(), (pixel*)texture.data.data(), view.width() * sizeof(pixel)));
}

void package_texture(sigma::resource::database<sigma::graphics::texture>& texture_database,
    const boost::filesystem::path& source_directory,
    texture_source_type source_type,
    const boost::filesystem::path& source_file)
{
    auto settings_path = source_file.parent_path() / (source_file.stem().string() + ".stex");

    auto rid = "texture" / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
    auto h = texture_database.handle_for({ rid });

    if (h.is_valid()) {
        auto source_file_time = boost::filesystem::last_write_time(source_file);
        auto settings_time = source_file_time;
        if (boost::filesystem::exists(settings_path))
            settings_time = boost::filesystem::last_write_time(settings_path);

        auto resource_time = texture_database.last_modification_time(h);
        if (source_file_time <= resource_time && settings_time <= resource_time)
            return;
    }

    std::cout << "packaging: " << rid << "\n";

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
        convert_texture<boost::gil::rgb8_image_t>(source_file, source_type, texture);
        break;
    }
    case sigma::graphics::texture_format::RGBA8: {
        convert_texture<boost::gil::rgba8_image_t>(source_file, source_type, texture);
        break;
    }
    case sigma::graphics::texture_format::RGB32F: {
        convert_texture<boost::gil::rgb32f_image_t>(source_file, source_type, texture);
        break;
    }
    }

    texture_database.insert({ rid }, texture);
}

void package_textures(
    sigma::resource::database<sigma::graphics::texture>& texture_database,
    const boost::filesystem::path& source_directory)
{
    static const std::unordered_map<std::string, texture_source_type> ext_to_type{
        { ".tiff", texture_source_type::tiff },
        { ".tif", texture_source_type::tiff },
        { ".jpg", texture_source_type::jpeg },
        { ".jpeg", texture_source_type::jpeg },
        { ".jpe", texture_source_type::jpeg },
        { ".jif", texture_source_type::jpeg },
        { ".jfif", texture_source_type::jpeg },
        { ".jfi", texture_source_type::jpeg },
        { ".png", texture_source_type::png },
        { ".hdr", texture_source_type::hdr }
    };

    boost::filesystem::recursive_directory_iterator it{ source_directory };
    boost::filesystem::recursive_directory_iterator end;
    for (; it != end; ++it) {
        auto path = it->path();
        if (sigma::filesystem::is_hidden(path)) {
            if (boost::filesystem::is_directory(path))
                it.no_push();
            continue;
        }

        auto ext = boost::algorithm::to_lower_copy(path.extension().string());
        auto ext_it = ext_to_type.find(ext);
        if (boost::filesystem::is_regular_file(path) && ext_it != ext_to_type.cend())
            package_texture(texture_database, source_directory, ext_it->second, path);
    }
}
