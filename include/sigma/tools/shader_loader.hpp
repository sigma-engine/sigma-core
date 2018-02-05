#ifndef SIGMA_TOOLS_SHADER_LOADER_HPP
#define SIGMA_TOOLS_SHADER_LOADER_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/tools/packager.hpp>
#include <sigma/util/filesystem.hpp>

#include <shaderc/shaderc.hpp>

#include <boost/algorithm/string.hpp>

#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

namespace sigma {
namespace tools {
    class file_includer : public shaderc::CompileOptions::IncluderInterface {
    public:
        file_includer(build_settings& settings)
            : settings_(settings)
        {
        }

        virtual shaderc_include_result* GetInclude(const char* requested_source,
            shaderc_include_type type,
            const char* requesting_source,
            size_t include_depth) override
        {
            boost::filesystem::path requested_path{ requested_source };
            boost::filesystem::path requesting_path{ requesting_source };

            boost::filesystem::path full_path = requesting_path.parent_path() / requested_path;
            if (!boost::filesystem::exists(full_path)) {
                for (const auto& base_path : settings_.source_directories) {
                    auto path = base_path / requested_path;
                    if (boost::filesystem::exists(path)) {
                        full_path = path;
                        break;
                    }
                }
                if (!boost::filesystem::exists(full_path))
                    return make_result("Include file does not exist.");
            }

            try {
                std::ifstream source{ full_path.string() };
                std::string source_code{
                    std::istreambuf_iterator<char>{ source.rdbuf() },
                    std::istreambuf_iterator<char>{}
                };
                return make_result(full_path.string(), source_code);
            } catch (...) {
                return make_result("Could not read include file.");
            }
            return nullptr;
        }

        virtual void ReleaseInclude(shaderc_include_result* result) override
        {
            auto data = static_cast<user_data*>(result->user_data);
            delete data;
            delete result;
        }

    private:
        build_settings& settings_;

        shaderc_include_result* make_result(const char* message)
        {
            return new shaderc_include_result{
                "", 0,
                message, strlen(message),
                nullptr
            };
        }

        shaderc_include_result* make_result(std::string filepath, std::string content)
        {
            auto data = new user_data{ filepath, content };
            return new shaderc_include_result{
                data->filepath.c_str(), data->filepath.size(),
                data->content.c_str(), data->content.size(),
                data
            };
        }

        struct user_data {
            std::string filepath;
            std::string content;
        };
    };

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
                ".vert",
                ".tesc",
                ".tese",
                ".geom",
                ".frag"
            };
            return supported_extensions.count(ext);
        }

        virtual void load(const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file) override
        {
            static const std::unordered_map<std::string, std::pair<sigma::graphics::shader_type, shaderc_shader_kind>> source_types = {
                { ".vert", { sigma::graphics::shader_type::vertex, shaderc_glsl_vertex_shader } },
                { ".tesc", { sigma::graphics::shader_type::tessellation_control, shaderc_glsl_tess_control_shader } },
                { ".tese", { sigma::graphics::shader_type::tessellation_evaluation, shaderc_glsl_tess_evaluation_shader } },
                { ".geom", { sigma::graphics::shader_type::geometry, shaderc_glsl_geometry_shader } },
                { ".frag", { sigma::graphics::shader_type::fragment, shaderc_glsl_fragment_shader } }
            };

            static const std::unordered_map<sigma::graphics::shader_type, std::string> type_name_map{
                { sigma::graphics::shader_type::vertex, "vertex" },
                { sigma::graphics::shader_type::tessellation_control, "tessellation_control" },
                { sigma::graphics::shader_type::tessellation_evaluation, "tessellation_evaluation" },
                { sigma::graphics::shader_type::geometry, "geometry" },
                { sigma::graphics::shader_type::fragment, "fragment" },
                { sigma::graphics::shader_type::header, "header" }
            };

            auto [source_type, shaderc_type] = source_types.at(ext);

            auto rid = type_name_map.at(source_type) / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");

            auto& shader_database = context_.template get_cache<graphics::shader>();
            if (shader_database.contains({ rid })) {
                auto h = shader_database.handle_for({ rid });

                auto source_file_time = boost::filesystem::last_write_time(source_file);
                auto resource_time = shader_database.last_modification_time(h);
                // TODO (NOW): other dependencies
                if (source_file_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << rid << "\n";

            sigma::graphics::shader shader;
            shader.type = source_type;

            std::ifstream source{ source_file.string() };
            std::string source_code{
                std::istreambuf_iterator<char>{ source.rdbuf() },
                std::istreambuf_iterator<char>{}
            };

            shaderc::Compiler compiler;
            shaderc::CompileOptions options;

            options.SetTargetEnvironment(shaderc_target_env_opengl, 0);
            options.SetIncluder(std::make_unique<file_includer>(settings_));

            switch (shader.type) {
            case sigma::graphics::shader_type::vertex: {
                options.AddMacroDefinition("SIGMA_ENGINE_VERTEX_SHADER");
                break;
            }
            case sigma::graphics::shader_type::tessellation_control: {
                options.AddMacroDefinition("SIGMA_ENGINE_TESSELLATION_CONTROL_SHADER", "1");
                break;
            }
            case sigma::graphics::shader_type::tessellation_evaluation: {
                options.AddMacroDefinition("SIGMA_ENGINE_TESSELLATION_EVALUATION_SHADER", "1");
                break;
            }
            case sigma::graphics::shader_type::geometry: {
                options.AddMacroDefinition("SIGMA_ENGINE_GEOMETRY_SHADER", "1");
                break;
            }
            case sigma::graphics::shader_type::fragment: {
                options.AddMacroDefinition("SIGMA_ENGINE_FRAGMENT_SHADER", "1");
                break;
            }
            case sigma::graphics::shader_type::header: {
                break;
            }
            }

            auto result = compiler.CompileGlslToSpv(source_code, shaderc_type, source_file.c_str(), options);
            if (result.GetCompilationStatus() != shaderc_compilation_status_success)
                throw std::runtime_error(result.GetErrorMessage());
            shader.spirv = std::vector<std::uint32_t>{ result.cbegin(), result.cend() };

            shader_database.insert({ rid }, shader, true);
        }

    private:
        build_settings& settings_;
        ContextType& context_;
    };
}
}

#endif // SIGMA_TOOLS_CUBEMAP_LOADER_HPP
