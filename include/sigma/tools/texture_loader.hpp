#ifndef SIGMA_TOOLS_TEXTURE_LOADER_HPP
#define SIGMA_TOOLS_TEXTURE_LOADER_HPP

#include <sigma/graphics/texture.hpp>
#include <sigma/tools/json_conversion.hpp>
#include <sigma/tools/packager.hpp>
#include <sigma/tools/texturing.hpp>

#include <stb/stb_image.h>

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
    void load_texture(const std::filesystem::path& source_path, Image& image)
    {
        auto file_path_string = source_path.string();
        int width, height, bbp;
        auto pixels = (typename Image::value_type*)stbi_load(file_path_string.c_str(), &width, &height, &bbp, sigma::graphics::channel_count_v<Image>);
        image.size = { width, height };
        image.pixels.resize(width * height);
        std::copy(pixels, pixels + (width * height), image.pixels.begin());
        stbi_image_free((void*)pixels);
    }

    void load_texture(const std::filesystem::path& source_path, sigma::graphics::image_t<sigma::graphics::rgb32f_pixel_t>& image)
    {
        auto file_path_string = source_path.string();
        int width, height, bbp;
        auto pixels = (sigma::graphics::rgb32f_pixel_t*)stbi_loadf(file_path_string.c_str(), &width, &height, &bbp, 3);
        image.size = { width, height };
        image.pixels.resize(width * height);
        std::copy(pixels, pixels + (width * height), image.pixels.begin());
        stbi_image_free((void*)pixels);
    }

    class texture_loader : public resource_loader {
    public:
        texture_loader(std::shared_ptr<context> ctx)
            : resource_loader(ctx)
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

        virtual void load(const std::filesystem::path& source_directory, const std::string& ext, const std::filesystem::path& source_file) override
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

            auto texture_cache = context_->cache<graphics::texture>();
            if (texture_cache->contains({ rid })) {
                auto h = texture_cache->handle_for({ rid });

                auto source_file_time = std::filesystem::last_write_time(source_file);
                auto settings_time = source_file_time;
                if (std::filesystem::exists(settings_path))
                    settings_time = std::filesystem::last_write_time(settings_path);

                auto resource_time = texture_cache->last_modification_time(h);
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

            if (std::filesystem::exists(settings_path)) {
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
                sigma::graphics::image_t<sigma::graphics::rgb8_pixel_t> image;
                load_texture(source_file, image);

                graphics::texture texture(image, minification_filter, magnification_filter, mipmap_filter);
                texture_cache->insert({ rid }, texture, true);
                break;
            }
            case graphics::texture_format::RGBA8: {
                sigma::graphics::image_t<sigma::graphics::rgba8_pixel_t> image;
                load_texture(source_file, image);

                graphics::texture texture(image, minification_filter, magnification_filter, mipmap_filter);
                texture_cache->insert({ rid }, texture, true);
                break;
            }
            case graphics::texture_format::RGB32F: {
                sigma::graphics::image_t<sigma::graphics::rgb32f_pixel_t> image;
                load_texture(source_file, image);

                if (settings.isMember("cubemap")) {
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

                        sigma::graphics::image_t<sigma::graphics::rgb32f_pixel_t> face_image({SIZE, SIZE});
                        equirectangular_to_cubemap_face(face, image, face_image);

                        graphics::texture face_texture(face_image, minification_filter, magnification_filter, mipmap_filter);

                        auto face_number = static_cast<unsigned int>(face);
                        cubemap.faces[face_number] = texture_cache->insert({ rid / std::to_string(face_number) }, face_texture, true);
                    }

                    auto cubemap_rid = resource_shortname(graphics::cubemap) / filesystem::make_relative(source_directory, source_file).replace_extension("");
                    context_->cache<graphics::cubemap>()->insert({ cubemap_rid }, cubemap, true);
                }

                // TODO do not convert and export texture if cubemap is generated?
                graphics::texture texture(image, minification_filter, magnification_filter, mipmap_filter);
                texture_cache->insert({ rid }, texture, true);
                break;
            }
            }
        }

    private:
        std::shared_ptr<context> context_;
    };
}
}

#endif // SIGMA_TOOLS_TEXTURE_LOADER_HPP
