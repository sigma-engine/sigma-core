#ifndef SIGMA_TOOLS_SHADER_LOADER_HPP
#define SIGMA_TOOLS_SHADER_LOADER_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/tools/packager.hpp>
#include <sigma/util/filesystem.hpp>

#include <boost/algorithm/string.hpp>

#include <sstream>
#include <string>
#include <unordered_map>

namespace sigma {
namespace tools {
    template <class ContextType>
    class shader_loader : public resource_loader<ContextType> {
    public:
        shader_loader(build_settings& settings, ContextType& ctx)
            : resource_loader<ContextType>(settings, ctx)
            , settings_(settings)
            , context_(ctx)
        {
        }

        virtual ~shader_loader()
        {
        }

        virtual bool supports_filetype(const std::string& ext) const override
        {
            static const std::set<std::string> supported_extensions = {
                ".vert.spv",
                ".tesc.spv",
                ".tese.spv",
                ".geom.spv",
                ".frag.spv"
            };
            return supported_extensions.count(ext);
        }

        virtual void load(const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file) override
        {
            static const std::unordered_map<std::string, sigma::graphics::shader_type> source_types = {
                { ".vert.spv", { sigma::graphics::shader_type::vertex } },
                { ".tesc.spv", { sigma::graphics::shader_type::tessellation_control } },
                { ".tese.spv", { sigma::graphics::shader_type::tessellation_evaluation } },
                { ".geom.spv", { sigma::graphics::shader_type::geometry } },
                { ".frag.spv", { sigma::graphics::shader_type::fragment } }
            };

            static const std::unordered_map<sigma::graphics::shader_type, std::string> type_name_map{
                { sigma::graphics::shader_type::vertex, "vertex" },
                { sigma::graphics::shader_type::tessellation_control, "tessellation_control" },
                { sigma::graphics::shader_type::tessellation_evaluation, "tessellation_evaluation" },
                { sigma::graphics::shader_type::geometry, "geometry" },
                { sigma::graphics::shader_type::fragment, "fragment" },
                { sigma::graphics::shader_type::header, "header" }
            };

            auto source_type = source_types.at(ext);

            auto rid = type_name_map.at(source_type) / sigma::filesystem::make_relative(source_directory, source_file);
            while(rid.has_extension())
                rid = rid.replace_extension();

            auto& shader_database = context_.template get_cache<graphics::shader>();
            if (shader_database.contains({ rid })) {
                auto h = shader_database.handle_for({ rid });

                auto source_file_time = boost::filesystem::last_write_time(source_file);
                auto resource_time = shader_database.last_modification_time(h);
                if (source_file_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << rid << "\n";

            sigma::graphics::shader shader;
            shader.type = source_type;

            std::ifstream source{ source_file.string() };
            shader.spirv.insert(shader.spirv.end(),
                std::istreambuf_iterator<char>{ source.rdbuf() },
                std::istreambuf_iterator<char>{});

            shader_database.insert({ rid }, shader, true);
        }

    private:
        build_settings& settings_;
        ContextType& context_;
    };
}
}

#endif // SIGMA_TOOLS_CUBEMAP_LOADER_HPP
