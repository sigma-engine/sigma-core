#ifndef SIGMA_TOOLS_TECHNIQUE_LOADER_HPP
#define SIGMA_TOOLS_TECHNIQUE_LOADER_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/reflect.hpp>
#include <sigma/tools/packager.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/json_conversion.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <set>
#include <string>

namespace sigma {
namespace tools {

    struct technique_shaders {
        BOOST_HANA_DEFINE_STRUCT(
            technique_shaders,
            (boost::filesystem::path, vertex),
            (boost::filesystem::path, tessellation_control),
            (boost::filesystem::path, tessellation_evaluation),
            (boost::filesystem::path, geometry),
            (boost::filesystem::path, fragment));

        complex_resource_id get_cid() const
        {
            complex_resource_id cid;
            cid.push_back("vertex" / vertex);
            if (tessellation_control.size() > 0)
                cid.push_back("tessellation_control" / tessellation_control);
            if (tessellation_evaluation.size() > 0)
                cid.push_back("tessellation_evaluation" / tessellation_evaluation);
            if (geometry.size() > 0)
                cid.push_back("geometry" / geometry);
            cid.push_back("fragment" / fragment);
            return cid;
        }

        std::size_t get_rid() const
        {
            return resource_id_for(get_cid());
        }

        std::string to_string() const
        {
            return "technique { " + boost::algorithm::join(get_cid() | boost::adaptors::transformed([](auto a) { return a.string(); }), ", ") + " }";
        }
    };

    void extract_unifrom_data(resource::cache<graphics::texture>& texture_cache,
        resource::cache<graphics::cubemap>& cubemap_cache,
        const Json::Value& settings, graphics::technique_uniform_data& data)
    {
        // TODO throw missing resource error if texture or cubemap were not found.

        for (auto it = settings.begin(); it != settings.end(); ++it) {
            const auto& value = *it;
            if (it.key() == "vertex" || it.key() == "tessellation_control" || it.key() == "tessellation_evaluation" || it.key() == "geometry" || it.key() == "fragment") {
                continue;
            } else if (it.key() == "textures") {
                const auto& texture_object = *it;
                for (auto it2 = texture_object.begin(); it2 != texture_object.end(); ++it2) {
                    auto rid = resource_id_for({ boost::filesystem::path{ "texture" } / (*it2).asString() });
                    data.textures[it2.key().asString()] = texture_cache.handle_for(rid); // TODO warn if tring to set texture more than once
                }
            } else if (it.key() == "cubemaps") {
                const auto& cubemap_object = *it;
                for (auto it2 = cubemap_object.begin(); it2 != cubemap_object.end(); ++it2) {
                    auto rid = resource_id_for({ boost::filesystem::path{ "cubemap" } / (*it2).asString() });
                    data.cubemaps[it2.key().asString()] = cubemap_cache.handle_for(rid); // TODO warn if tring to set cubemap more than once
                }
            } else {
                float f;
                glm::vec2 v2;
                glm::vec3 v3;
                glm::vec4 v4;
                if (json::from_json(value, v4))
                    data.vec4s[it.key().asString()] = v4;
                else if (json::from_json(value, v3))
                    data.vec3s[it.key().asString()] = v3;
                else if (json::from_json(value, v2))
                    data.vec2s[it.key().asString()] = v2;
                else if (json::from_json(value, f))
                    data.floats[it.key().asString()] = f;
            }
        }
    }

    template <class ContextType>
    class technique_loader : public resource_loader<ContextType> {
    public:
        technique_loader(ContextType& ctx)
            : context_{ ctx }
        {
        }

        virtual ~technique_loader() = default;

        virtual bool supports_filetype(const std::string& ext) const override
        {
            static const std::set<std::string> supported_extensions = {
                ".stech",
                ".smat",
                ".eff"
            };
            return supported_extensions.count(ext) > 0;
        }

        virtual void load(const Json::Value& global_settings, const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file) override
        {
            auto& shader_cache = context_.template get_cache<graphics::shader>();
            auto& technique_cache = context_.template get_cache<graphics::technique>();

            std::ifstream file(source_file.string());
            Json::Value settings;
            file >> settings;

            technique_shaders shaders;
            json::from_json(settings, shaders);

            auto rid = shaders.get_rid();
            if (technique_cache.contains(rid)) {
                auto h = technique_cache.handle_for(rid);

                auto source_file_time = boost::filesystem::last_write_time(source_file);
                auto resource_time = technique_cache.last_modification_time(h);
                // TODO (NOW): other dependencies
                if (source_file_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << shaders.to_string() << "\n";

            graphics::technique technique;
            technique.shaders.vertex = shader_cache.handle_for(resource_id_for({ "vertex" / shaders.vertex }));
            if (shaders.tessellation_control.size() > 0)
                technique.shaders.tessellation_control = shader_cache.handle_for(resource_id_for({ "tessellation_control" / shaders.tessellation_control }));
            if (shaders.tessellation_evaluation.size() > 0)
                technique.shaders.tessellation_evaluation = shader_cache.handle_for(resource_id_for({ "tessellation_evaluation" / shaders.tessellation_evaluation }));
            if (shaders.geometry.size() > 0)
                technique.shaders.geometry = shader_cache.handle_for(resource_id_for({ "geometry" / shaders.geometry }));
            technique.shaders.fragment = shader_cache.handle_for(resource_id_for({ "fragment" / shaders.fragment }));

            // TODO: (NOW) check for link errors.
            // TODO: (NOW) extract reflection data.

            technique_cache.insert(rid, technique, true);
        }

    private:
        ContextType& context_;
    };
}
}

#endif // SIGMA_TOOLS_TECHNIQUE_MODEL_HPP
