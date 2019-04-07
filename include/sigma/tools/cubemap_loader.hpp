#ifndef SIGMA_TOOLS_CUBEMAP_LOADER_HPP
#define SIGMA_TOOLS_CUBEMAP_LOADER_HPP

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/tools/packager.hpp>

#include <set>
#include <string>

namespace sigma {
namespace tools {

    class cubemap_loader : public resource_loader {
    public:
        cubemap_loader(std::shared_ptr<context> ctx)
            : resource_loader(ctx)
            , context_(ctx)
        {
        }

        virtual ~cubemap_loader() = default;

        virtual bool supports_filetype(const std::string& ext) const override
        {
            static const std::set<std::string> supported_extensions = {
                ".cub"
            };
            return supported_extensions.count(ext) > 0;
        }

        virtual void load(const std::filesystem::path& source_directory, const std::string& ext, const std::filesystem::path& source_file) override
        {
            auto rid = resource_shortname(sigma::graphics::cubemap) / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");

            auto cubemap_cache = context_->cache<graphics::cubemap>();
            if (cubemap_cache->contains({ rid })) {
                auto h = cubemap_cache->handle_for({ rid });

                auto source_file_time = std::filesystem::last_write_time(source_file);
                auto resource_time = cubemap_cache->last_modification_time(h);
                // TODO(NOW): other dependencies
                if (source_file_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << rid << "\n";

            Json::Value settings;
            std::ifstream file(source_file.string());
            file >> settings;

            sigma::graphics::cubemap cubemap;

            // TODO remove right, left, top, bottom, back, front and use
            // {positive,negative}_{x,y,z}
            static const std::pair<std::string, graphics::cubemap::face> face_names[] = {
                { "right", graphics::cubemap::face::POSITIVE_X },
                { "left", graphics::cubemap::face::NEGATIVE_X },
                { "top", graphics::cubemap::face::POSITIVE_Y },
                { "bottom", graphics::cubemap::face::NEGATIVE_Y },
                { "front", graphics::cubemap::face::POSITIVE_Z },
                { "back", graphics::cubemap::face::NEGATIVE_Z }
            };
            for (const auto& face_name : face_names)
                json::from_json(context_, settings[face_name.first].asString(), cubemap.faces[static_cast<unsigned int>(face_name.second)]);

            // TODO: throw better error message if missing a face.
            cubemap_cache->insert({ rid }, cubemap, true);
        }

    private:
        std::shared_ptr<context> context_;
    };
}
}

#endif // SIGMA_TOOLS_CUBEMAP_LOADER_HPP
