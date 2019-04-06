#ifndef SIGMA_TOOLS_TEXTURE_LOADER_HPP
#define SIGMA_TOOLS_TEXTURE_LOADER_HPP

#include <sigma/graphics/texture.hpp>
#include <sigma/tools/hdr_io.hpp>
#include <sigma/tools/json_conversion.hpp>
#include <sigma/tools/packager.hpp>
#include <sigma/tools/texturing.hpp>

#include <boost/gil/extension/io/tiff/read.hpp>
#include <boost/gil/extension/io/jpeg/read.hpp>
#include <boost/gil/extension/io/png/read.hpp>

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
            // boost::gil::tiff_read_and_convert_image(file_path_string, image);
            boost::gil::read_and_convert_image(file_path_string, image, boost::gil::tiff_tag());
            break;
        }
        case texture_source_type::jpeg: {
            boost::gil::read_and_convert_image(file_path_string, image, boost::gil::jpeg_tag());
            // boost::gil::jpeg_read_and_convert_image(file_path_string, image);
            break;
        }
        case texture_source_type::png: {
            boost::gil::read_and_convert_image(file_path_string, image, boost::gil::png_tag());
            // boost::gil::png_read_and_convert_image(file_path_string, image);
            break;
        }
        case texture_source_type::hdr: {
            hdr_read_and_convert_image(file_path_string, image);
            break;
        }
        }
    }

    template <class ContextType>
    class texture_loader : public resource_loader<ContextType> {
    public:
        texture_loader(build_settings& settings, ContextType& ctx)
            : resource_loader<ContextType>(settings, ctx)
            , context_(ctx)
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

        virtual void load(const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file) override
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

            auto rid = resource_shortname(graphics::texture) / filesystem::make_relative(source_directory, source_file).replace_extension("");

            auto& texture_cache = context_.template get_cache<graphics::texture>();
            if (texture_cache.contains({ rid })) {
                auto h = texture_cache.handle_for({ rid });

                auto source_file_time = boost::filesystem::last_write_time(source_file);
                auto settings_time = source_file_time;
                if (boost::filesystem::exists(settings_path))
                    settings_time = boost::filesystem::last_write_time(settings_path);

                auto resource_time = texture_cache.last_modification_time(h);
                if (source_file_time <= resource_time && settings_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << rid << "\n";

            // TODO use json conversion
            Json::Value settings(Json::objectValue);
            settings["format"] = "rgb8";
            settings["filter"]["minification"] = "linear";
            settings["filter"]["magnification"] = "linear";
            settings["filter"]["mipmap"] = "linear";

            if (boost::filesystem::exists(settings_path)) {
                std::ifstream file(settings_path.string());
                file >> settings;
            }

            graphics::texture_filter minification_filter = graphics::texture_filter::LINEAR;
            graphics::texture_filter magnification_filter = graphics::texture_filter::LINEAR;
            graphics::texture_filter mipmap_filter = graphics::texture_filter::LINEAR;
            graphics::texture_format format = graphics::texture_format::RGB8;
            json::from_json(context_, settings["filter"]["minification"], minification_filter);
            json::from_json(context_, settings["filter"]["magnification"], magnification_filter);
            json::from_json(context_, settings["filter"]["mipmap"], mipmap_filter);
            json::from_json(context_, settings["format"], format);

            switch (format) {
            case graphics::texture_format::RGB8: {
                boost::gil::rgb8_image_t image;
                load_texture(source_file, source_type, image);

                graphics::texture texture(boost::gil::const_view(image), minification_filter, magnification_filter, mipmap_filter);
                texture_cache.insert({ rid }, texture, true);
                break;
            }
            case graphics::texture_format::RGBA8: {
                boost::gil::rgba8_image_t image;
                load_texture(source_file, source_type, image);

                graphics::texture texture(boost::gil::const_view(image), minification_filter, magnification_filter, mipmap_filter);
                texture_cache.insert({ rid }, texture, true);
                break;
            }
            case graphics::texture_format::RGB32F: {
                boost::gil::rgb32f_image_t image;
                load_texture(source_file, source_type, image);

                if (settings.isMember("cubemap")) {
                    auto source_view = boost::gil::const_view(image);
                    graphics::cubemap cubemap;

                    int SIZE = 1024;
                    if (settings["cubemap"].isMember("size"))
                        SIZE = settings["cubemap"]["size"].asUInt();

                    for (auto face : { graphics::cubemap::face::POSITIVE_X,
                             graphics::cubemap::face::NEGATIVE_X,
                             graphics::cubemap::face::POSITIVE_Y,
                             graphics::cubemap::face::NEGATIVE_Y,
                             graphics::cubemap::face::POSITIVE_Z,
                             graphics::cubemap::face::NEGATIVE_Z }) {

                        graphics::texture face_texture({ SIZE, SIZE }, format, minification_filter, magnification_filter, mipmap_filter);
                        auto face_view = face_texture.as_view<boost::gil::rgb32f_pixel_t>(0);
                        equirectangular_to_cubemap_face(face, source_view, face_view);

                        auto face_number = static_cast<unsigned int>(face);
                        cubemap.faces[face_number] = texture_cache.insert({ rid / std::to_string(face_number) }, face_texture, true);
                    }

                    auto cubemap_rid = resource_shortname(graphics::cubemap) / filesystem::make_relative(source_directory, source_file).replace_extension("");
                    context_.template get_cache<graphics::cubemap>().insert({ cubemap_rid }, cubemap, true);
                }

                // TODO do not convert and export texture if cubemap is generated?
                graphics::texture texture(boost::gil::const_view(image), minification_filter, magnification_filter, mipmap_filter);
                texture_cache.insert({ rid }, texture, true);
                break;
            }
            }
        }

    private:
        ContextType& context_;
    };
}
}

#endif // SIGMA_TOOLS_TEXTURE_LOADER_HPP
