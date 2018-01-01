#ifndef SIGMA_TOOLS_POST_PROCESS_EFFECT_LOADER_HPP
#define SIGMA_TOOLS_POST_PROCESS_EFFECT_LOADER_HPP

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/tools/packager.hpp>
#include <sigma/tools/technique_loader.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/json_conversion.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <set>
#include <string>

namespace sigma {
namespace tools {

    template <class ContextType>
    class post_process_effect_loader : public resource_loader<ContextType> {
    public:
        post_process_effect_loader(ContextType& ctx)
            : context_{ ctx }
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

        virtual void load(const Json::Value& global_settings, const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file) override
        {
            auto& texture_cache = context_.template get_cache<graphics::texture>();
            auto& cubemap_cache = context_.template get_cache<graphics::cubemap>();
            auto& technique_cache = context_.template get_cache<graphics::technique>();
            auto& static_mesh_cache = context_.template get_cache<graphics::static_mesh>();
            auto& post_process_effect_cache = context_.template get_cache<graphics::post_process_effect>();

            auto cid = resource_shortname(sigma::graphics::post_process_effect) / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
            auto rid = resource_id_for({ cid });

            if (post_process_effect_cache.contains(rid)) {
                auto h = post_process_effect_cache.handle_for(rid);

                auto source_file_time = boost::filesystem::last_write_time(source_file);
                auto resource_time = post_process_effect_cache.last_modification_time(h);
                // TODO (NOW): other dependencies
                if (source_file_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << cid << "\n";

            std::ifstream file(source_file.string());
            Json::Value settings;
            file >> settings;

            technique_shaders shaders;
            json::from_json(settings, shaders);

            sigma::graphics::post_process_effect effect;
            effect.technique_id = technique_cache.handle_for(shaders.get_rid());

            extract_unifrom_data(texture_cache, cubemap_cache, settings, effect);

            auto mesh_prefix = resource_shortname(graphics::static_mesh);
            auto mesh_cid = boost::filesystem::path{ mesh_prefix } / "fullscreen_quad";
            if (settings.isMember(mesh_prefix))
                mesh_cid = boost::filesystem::path{ mesh_prefix } / settings[mesh_prefix].asString();

            effect.mesh = static_mesh_cache.handle_for(resource_id_for({ mesh_cid }));

            post_process_effect_cache.insert(rid, effect, true);
        }
    private:
        ContextType& context_;
    };
}
}

#endif // SIGMA_TOOLS_POST_PROCESS_EFFECT_MODEL_HPP
