#ifndef SIGMA_TOOLS_MATERIAL_LOADER_HPP
#define SIGMA_TOOLS_MATERIAL_LOADER_HPP

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/tools/packager.hpp>

#include <sigma/tools/json_conversion.hpp>
#include <sigma/tools/technique_loader.hpp>
#include <sigma/util/filesystem.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <set>
#include <string>

namespace sigma {
namespace tools {

    template <class PackageSettings, class ContextType>
    class material_loader : public resource_loader<PackageSettings, ContextType> {
    public:
        material_loader(ContextType& ctx)
            : context_{ ctx }
        {
        }

        virtual ~material_loader() = default;

        virtual bool supports_filetype(const std::string& ext) const override
        {
            static const std::set<std::string> supported_extensions = {
                ".smat"
            };
            return supported_extensions.count(ext) > 0;
        }

        virtual void load(const PackageSettings& package_settings, const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file) override
        {
            auto& technique_cache = context_.template get_cache<graphics::technique>();
            auto& material_cache = context_.template get_cache<graphics::material>();

            auto cid = resource_shortname(sigma::graphics::material) / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
            auto rid = resource_id_for({ cid });

            if (material_cache.contains(rid)) {
                auto h = material_cache.handle_for(rid);

                auto source_file_time = boost::filesystem::last_write_time(source_file);
                auto resource_time = material_cache.last_modification_time(h);
                // TODO (NOW): other dependencies
                if (source_file_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << cid << "\n";

            std::ifstream file(source_file.string());
            Json::Value settings;
            file >> settings;

            graphics::technique_identifier tech_id;
            json::from_json(context_, settings, tech_id);

            // TODO add any other other shaders here.
            complex_resource_id tech_cid{
                tech_id.vertex,
                tech_id.tessellation_control,
                tech_id.tessellation_evaluation,
                tech_id.geometry,
                tech_id.fragment
            };
            sigma::graphics::material material;
            material.technique_id = technique_cache.handle_for(resource_id_for(tech_cid));

            extract_unifrom_data(context_, settings, material);

            material_cache.insert(rid, material, true);
        }

    private:
        ContextType& context_;
    };
}
}

#endif // SIGMA_TOOLS_MATERIAL_MODEL_HPP
