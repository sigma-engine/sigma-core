#ifndef SIGMA_TOOLS_TEXTURE_LOADER_HPP
#define SIGMA_TOOLS_TEXTURE_LOADER_HPP

#include <sigma/graphics/texture.hpp>
#include <sigma/tools/hdr_io.hpp>
#include <sigma/tools/json_conversion.hpp>
#include <sigma/tools/packager.hpp>

#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>

#include <string>
#include <unordered_map>

namespace sigma {
namespace tools {
    enum class texture_source_type : unsigned int {
        tiff,
        jpeg,
        png,
        hdr
    };

    template <class Image>
    void load_texture(const boost::filesystem::path& source_file, texture_source_type source_type, Image& image)
    {
        auto file_path_string = source_file.string();

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
    }

    template <class Image>
    void convert_texture(const Image& image, graphics::texture& texture)
    {
        auto view = boost::gil::const_view(image);
        using pixel = typename decltype(view)::value_type;
        texture.data.resize(image.width() * image.height() * sizeof(pixel));
        texture.size = glm::ivec2{ image.width(), image.height() };
        boost::gil::copy_pixels(view, boost::gil::interleaved_view(view.width(), view.height(), (pixel*)texture.data.data(), view.width() * sizeof(pixel)));
    }

    template <class PackageSettings, class ContextType>
    class texture_loader : public resource_loader<PackageSettings, ContextType> {
    public:
        texture_loader(ContextType& ctx)
            : context_{ ctx }
        {
        }

        virtual ~texture_loader() = default;

        virtual bool supports_filetype(const std::string& ext) const override
        {
            static const std::set<std::string> supported_extensions = {
                ".tiff",
                ".tif",
                ".jpg",
                ".jpeg",
                ".jpe",
                ".jif",
                ".jfif",
                ".jfi",
                ".png",
                ".hdr"
            };
            return supported_extensions.count(ext) > 0;
        }

        virtual void load(const PackageSettings& package_settings, const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file) override
        {
            static const std::unordered_map<std::string, texture_source_type> type_map = {
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

            auto source_type = type_map.at(ext);

            auto settings_path = source_file.parent_path() / (source_file.stem().string() + ".stex");

            auto cid = resource_shortname(graphics::texture) / filesystem::make_relative(source_directory, source_file).replace_extension("");
            auto rid = resource_id_for({ cid });

            auto& texture_cache = context_.template get_cache<graphics::texture>();
            if (texture_cache.contains(rid)) {
                auto h = texture_cache.handle_for(rid);

                auto source_file_time = boost::filesystem::last_write_time(source_file);
                auto settings_time = source_file_time;
                if (boost::filesystem::exists(settings_path))
                    settings_time = boost::filesystem::last_write_time(settings_path);

                auto resource_time = texture_cache.last_modification_time(h);
                if (source_file_time <= resource_time && settings_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << cid << "\n";

            Json::Value settings(Json::objectValue);
            settings["format"] = "rgb8";
            settings["filter"]["minification"] = "linear";
            settings["filter"]["magnification"] = "linear";
            settings["filter"]["mipmap"] = "linear";

            if (boost::filesystem::exists(settings_path)) {
                std::ifstream file(settings_path.string());
                file >> settings;
            }

            graphics::texture texture;
            json::from_json(context_, settings["filter"]["minification"], texture.minification_filter);
            json::from_json(context_, settings["filter"]["magnification"], texture.magnification_filter);
            json::from_json(context_, settings["filter"]["mipmap"], texture.mipmap_filter);
            json::from_json(context_, settings["format"], texture.format);

            switch (texture.format) {
            case graphics::texture_format::RGB8: {
                boost::gil::rgb8_image_t image;
                load_texture(source_file, source_type, image);
                convert_texture(image, texture);
                break;
            }
            case graphics::texture_format::RGBA8: {
                boost::gil::rgba8_image_t image;
                load_texture(source_file, source_type, image);
                convert_texture(image, texture);
                break;
            }
            case graphics::texture_format::RGB32F: {
                boost::gil::rgb32f_image_t image;
                load_texture(source_file, source_type, image);
                convert_texture(image, texture);
                break;
            }
            }

            texture_cache.insert(rid, texture, true);
        }

    private:
        ContextType& context_;
    };
}
}

#endif // SIGMA_TOOLS_TEXTURE_LOADER_HPP
