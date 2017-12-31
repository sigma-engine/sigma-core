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
        using context_view_type = context_view<graphics::texture, graphics::cubemap>;

        cubemap_loader(context_view_type ctx)
            : context_{ ctx }
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

        virtual void load(const Json::Value& global_settings, const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file) override
        {
            auto cid = resource_shortname(sigma::graphics::cubemap) / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
            auto rid = resource_id_for({ cid });

            auto& cubemap_cache = context_.get_cache<graphics::cubemap>();
            auto& texture_cache = context_.get_cache<graphics::texture>();
            if (cubemap_cache.contains(rid)) {
                auto h = cubemap_cache.handle_for(rid);

                auto source_file_time = boost::filesystem::last_write_time(source_file);
                auto resource_time = cubemap_cache.last_modification_time(h);
                // TODO(NOW): other dependencies
                if (source_file_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << cid << "\n";

            Json::Value settings;

            std::ifstream file(source_file.string());
            file >> settings;

            sigma::graphics::cubemap cubemap;
            // TODO: (NOW) error if can not find textures.
            // TODO: (NOW) throw if missing a face.
            // TODO: (NOW) use json conversion.
            boost::filesystem::path texture_prefix{ resource_shortname(sigma::graphics::texture) };

            cubemap.right = texture_cache.handle_for(resource_id_for({ texture_prefix / settings["right"].asString() }));
            cubemap.left = texture_cache.handle_for(resource_id_for({ texture_prefix / settings["left"].asString() }));
            cubemap.top = texture_cache.handle_for(resource_id_for({ texture_prefix / settings["top"].asString() }));
            cubemap.bottom = texture_cache.handle_for(resource_id_for({ texture_prefix / settings["bottom"].asString() }));
            cubemap.back = texture_cache.handle_for(resource_id_for({ texture_prefix / settings["back"].asString() }));
            cubemap.front = texture_cache.handle_for(resource_id_for({ texture_prefix / settings["front"].asString() }));

            cubemap_cache.insert(rid, cubemap, true);
        }

    private:
        context_view_type context_;
    };
}
}

#endif // SIGMA_TOOLS_CUBEMAP_LOADER_HPP
