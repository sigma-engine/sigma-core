#ifndef SIGMA_TOOLS_POST_PROCESS_EFFECT_LOADER_HPP
#define SIGMA_TOOLS_POST_PROCESS_EFFECT_LOADER_HPP

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/tools/json_conversion.hpp>
#include <sigma/tools/packager.hpp>
#include <sigma/tools/technique_loader.hpp>
#include <sigma/util/filesystem.hpp>

#include <set>
#include <string>

namespace sigma {
namespace tools {

    class post_process_effect_loader : public resource_loader {
    public:
        post_process_effect_loader(std::shared_ptr<context> ctx)
            : resource_loader(ctx)
            , context_(ctx)
        {
        }

        virtual ~post_process_effect_loader() = default;

        virtual bool supports_filetype(const std::string& ext) const override
        {
            static const std::set<std::string> supported_extensions = {
                ".eff"
            };
            return supported_extensions.count(ext) > 0;
        }

        virtual void load(const std::filesystem::path& source_directory, const std::string& ext, const std::filesystem::path& source_file) override
        {
            auto technique_cache = context_->cache<graphics::technique>();
            auto static_mesh_cache = context_->cache<graphics::static_mesh>();
            auto post_process_effect_cache = context_->cache<graphics::post_process_effect>();

            auto rid = resource_shortname(sigma::graphics::post_process_effect) / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");

            if (post_process_effect_cache->contains({ rid })) {
                auto h = post_process_effect_cache->handle_for({ rid });

                auto source_file_time = std::filesystem::last_write_time(source_file);
                auto resource_time = post_process_effect_cache->last_modification_time(h);
                // TODO (NOW): other dependencies
                if (source_file_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << rid << "\n";

            std::ifstream file(source_file.string());
            Json::Value settings;
            file >> settings;

            graphics::technique_identifier tech_id;
            json::from_json(context_, settings, tech_id);

            // TODO add any other other shaders here.
            resource::resource_id tech_rid{
                tech_id.vertex,
                tech_id.tessellation_control,
                tech_id.tessellation_evaluation,
                tech_id.geometry,
                tech_id.fragment
            };
            sigma::graphics::post_process_effect effect;
            effect.technique_id = technique_cache->handle_for(tech_rid);

            extract_unifrom_data(context_, settings, effect);

            auto mesh_prefix = resource_shortname(graphics::static_mesh);
            auto mesh_rid = std::filesystem::path{ mesh_prefix } / "fullscreen_quad";
            if (settings.isMember(mesh_prefix))
                mesh_rid = std::filesystem::path{ mesh_prefix } / settings[mesh_prefix].asString();

            effect.mesh = static_mesh_cache->handle_for({ mesh_rid });

            post_process_effect_cache->insert({ rid }, effect, true);
        }

    private:
        std::shared_ptr<context> context_;
    };
}
}

#endif // SIGMA_TOOLS_POST_PROCESS_EFFECT_MODEL_HPP
