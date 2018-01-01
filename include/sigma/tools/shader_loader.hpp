#ifndef SIGMA_TOOLS_SHADER_LOADER_HPP
#define SIGMA_TOOLS_SHADER_LOADER_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/tools/packager.hpp>
#include <sigma/util/filesystem.hpp>

#include <glslang/MachineIndependent/reflection.h>
#include <glslang/Public/ShaderLang.h>

#include <boost/algorithm/string.hpp>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

namespace sigma {
namespace tools {
    class glsl_preprocessing_hooks : public boost::wave::context_policies::default_preprocessing_hooks {
    public:
        std::vector<boost::filesystem::path> source_files;

        template <typename ContextT, typename ContainerT>
        bool found_unknown_directive(ContextT const& ctx, ContainerT const& line, ContainerT& pending)
        {
            auto it = line.begin();
            auto id = boost::wave::util::impl::skip_whitespace(it, line.end());

            if (id != boost::wave::T_IDENTIFIER)
                return false;

            if (it->get_value() == "version" || it->get_value() == "extension") {
                std::copy(line.begin(), line.end(), std::back_inserter(pending));
                return true;
            }

            return false;
        }

        template <typename ContextT, typename ContainerT>
        bool emit_line_directive(ContextT const& ctx, ContainerT& pending, typename ContextT::token_type const& act_token)
        {
            auto position = act_token.get_position();
            std::string file_name{ position.get_file().c_str() };

            auto it = std::find(source_files.begin(), source_files.end(), file_name);
            std::size_t file_index = it - source_files.begin();
            std::size_t size = source_files.size();
            if (file_index >= size) {
                file_index = size;
                source_files.push_back(position.get_file().c_str());
            }

            auto line = "#line " + std::to_string(position.get_line()) + " " + std::to_string(file_index) + "\n";
            pending.push_back(boost::wave::cpplexer::lex_token<>(boost::wave::T_PP_LINE, line.c_str(), position));

            return true;
        }
    };

    const TBuiltInResource DefaultTBuiltInResource = {
        /* .MaxLights = */ 32,
        /* .MaxClipPlanes = */ 6,
        /* .MaxTextureUnits = */ 32,
        /* .MaxTextureCoords = */ 32,
        /* .MaxVertexAttribs = */ 64,
        /* .MaxVertexUniformComponents = */ 4096,
        /* .MaxVaryingFloats = */ 64,
        /* .MaxVertexTextureImageUnits = */ 32,
        /* .MaxCombinedTextureImageUnits = */ 80,
        /* .MaxTextureImageUnits = */ 32,
        /* .MaxFragmentUniformComponents = */ 4096,
        /* .MaxDrawBuffers = */ 32,
        /* .MaxVertexUniformVectors = */ 128,
        /* .MaxVaryingVectors = */ 8,
        /* .MaxFragmentUniformVectors = */ 16,
        /* .MaxVertexOutputVectors = */ 16,
        /* .MaxFragmentInputVectors = */ 15,
        /* .MinProgramTexelOffset = */ -8,
        /* .MaxProgramTexelOffset = */ 7,
        /* .MaxClipDistances = */ 8,
        /* .MaxComputeWorkGroupCountX = */ 65535,
        /* .MaxComputeWorkGroupCountY = */ 65535,
        /* .MaxComputeWorkGroupCountZ = */ 65535,
        /* .MaxComputeWorkGroupSizeX = */ 1024,
        /* .MaxComputeWorkGroupSizeY = */ 1024,
        /* .MaxComputeWorkGroupSizeZ = */ 64,
        /* .MaxComputeUniformComponents = */ 1024,
        /* .MaxComputeTextureImageUnits = */ 16,
        /* .MaxComputeImageUniforms = */ 8,
        /* .MaxComputeAtomicCounters = */ 8,
        /* .MaxComputeAtomicCounterBuffers = */ 1,
        /* .MaxVaryingComponents = */ 60,
        /* .MaxVertexOutputComponents = */ 64,
        /* .MaxGeometryInputComponents = */ 64,
        /* .MaxGeometryOutputComponents = */ 128,
        /* .MaxFragmentInputComponents = */ 128,
        /* .MaxImageUnits = */ 8,
        /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
        /* .MaxCombinedShaderOutputResources = */ 8,
        /* .MaxImageSamples = */ 0,
        /* .MaxVertexImageUniforms = */ 0,
        /* .MaxTessControlImageUniforms = */ 0,
        /* .MaxTessEvaluationImageUniforms = */ 0,
        /* .MaxGeometryImageUniforms = */ 0,
        /* .MaxFragmentImageUniforms = */ 8,
        /* .MaxCombinedImageUniforms = */ 8,
        /* .MaxGeometryTextureImageUnits = */ 16,
        /* .MaxGeometryOutputVertices = */ 256,
        /* .MaxGeometryTotalOutputComponents = */ 1024,
        /* .MaxGeometryUniformComponents = */ 1024,
        /* .MaxGeometryVaryingComponents = */ 64,
        /* .MaxTessControlInputComponents = */ 128,
        /* .MaxTessControlOutputComponents = */ 128,
        /* .MaxTessControlTextureImageUnits = */ 16,
        /* .MaxTessControlUniformComponents = */ 1024,
        /* .MaxTessControlTotalOutputComponents = */ 4096,
        /* .MaxTessEvaluationInputComponents = */ 128,
        /* .MaxTessEvaluationOutputComponents = */ 128,
        /* .MaxTessEvaluationTextureImageUnits = */ 16,
        /* .MaxTessEvaluationUniformComponents = */ 1024,
        /* .MaxTessPatchComponents = */ 120,
        /* .MaxPatchVertices = */ 32,
        /* .MaxTessGenLevel = */ 64,
        /* .MaxViewports = */ 16,
        /* .MaxVertexAtomicCounters = */ 0,
        /* .MaxTessControlAtomicCounters = */ 0,
        /* .MaxTessEvaluationAtomicCounters = */ 0,
        /* .MaxGeometryAtomicCounters = */ 0,
        /* .MaxFragmentAtomicCounters = */ 8,
        /* .MaxCombinedAtomicCounters = */ 8,
        /* .MaxAtomicCounterBindings = */ 1,
        /* .MaxVertexAtomicCounterBuffers = */ 0,
        /* .MaxTessControlAtomicCounterBuffers = */ 0,
        /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
        /* .MaxGeometryAtomicCounterBuffers = */ 0,
        /* .MaxFragmentAtomicCounterBuffers = */ 1,
        /* .MaxCombinedAtomicCounterBuffers = */ 1,
        /* .MaxAtomicCounterBufferSize = */ 16384,
        /* .MaxTransformFeedbackBuffers = */ 4,
        /* .MaxTransformFeedbackInterleavedComponents = */ 64,
        /* .MaxCullDistances = */ 8,
        /* .MaxCombinedClipAndCullDistances = */ 8,
        /* .MaxSamples = */ 4,
        /* .limits = */ {
            /* .nonInductiveForLoops = */ 1,
            /* .whileLoops = */ 1,
            /* .doWhileLoops = */ 1,
            /* .generalUniformIndexing = */ 1,
            /* .generalAttributeMatrixVectorIndexing = */ 1,
            /* .generalVaryingIndexing = */ 1,
            /* .generalSamplerIndexing = */ 1,
            /* .generalVariableIndexing = */ 1,
            /* .generalConstantMatrixVectorIndexing = */ 1,
        }
    };

    typedef boost::wave::cpplexer::lex_token<> token_type;
    typedef boost::wave::cpplexer::lex_iterator<token_type> lex_iterator_type;
    typedef boost::wave::context<std::string::iterator, lex_iterator_type, boost::wave::iteration_context_policies::load_file_to_string, glsl_preprocessing_hooks> wave_context_type;

    template <class PackageSettings, class ContextType>
    class shader_loader : public resource_loader<PackageSettings, ContextType> {
    public:
        shader_loader(ContextType& ctx)
            : context_{ ctx }
        {
            glslang::InitializeProcess();
        }

        virtual ~shader_loader()
        {
            glslang::FinalizeProcess();
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

        virtual void load(const PackageSettings& package_settings, const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file) override
        {
            static const std::unordered_map<std::string, sigma::graphics::shader_type> source_types = {
                { ".vert", sigma::graphics::shader_type::vertex },
                { ".tesc", sigma::graphics::shader_type::geometry },
                { ".tese", sigma::graphics::shader_type::geometry },
                { ".geom", sigma::graphics::shader_type::geometry },
                { ".frag", sigma::graphics::shader_type::fragment }
            };
            static const std::unordered_map<sigma::graphics::shader_type, std::string> type_name_map{
                { sigma::graphics::shader_type::vertex, "vertex" },
                { sigma::graphics::shader_type::geometry, "tessellation_control" },
                { sigma::graphics::shader_type::geometry, "tessellation_evaluation" },
                { sigma::graphics::shader_type::geometry, "geometry" },
                { sigma::graphics::shader_type::fragment, "fragment" },
                { sigma::graphics::shader_type::header, "header" }
            };
            static const std::unordered_map<sigma::graphics::shader_type, EShLanguage> shader_types_map{
                { sigma::graphics::shader_type::vertex, EShLangVertex },
                { sigma::graphics::shader_type::tessellation_control, EShLangTessControl },
                { sigma::graphics::shader_type::tessellation_evaluation, EShLangTessEvaluation },
                { sigma::graphics::shader_type::geometry, EShLangGeometry },
                { sigma::graphics::shader_type::fragment, EShLangFragment }
            };

            auto source_type = source_types.at(ext);

            auto cid = type_name_map.at(source_type) / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
            auto rid = resource_id_for({ cid });

            auto& shader_database = context_.template get_cache<graphics::shader>();
            if (shader_database.contains(rid)) {
                auto h = shader_database.handle_for(rid);

                auto source_file_time = boost::filesystem::last_write_time(source_file);
                auto resource_time = shader_database.last_modification_time(h);
                // TODO (NOW): other dependencies
                if (source_file_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << cid << "\n";

            sigma::graphics::shader shader;
            shader.type = source_type;

            std::ifstream source{ source_file.string() };
            std::string source_code{
                std::istreambuf_iterator<char>{ source.rdbuf() },
                std::istreambuf_iterator<char>{}
            };

            wave_context_type ctx{
                source_code.begin(),
                source_code.end(),
                source_file.c_str()
            };
            ctx.get_hooks().source_files.push_back(source_file);

            ctx.add_macro_definition("SIGMA_ENGINE_SHADER");
            switch (shader.type) {
            case sigma::graphics::shader_type::vertex: {
                ctx.add_macro_definition("SIGMA_ENGINE_VERTEX_SHADER");
                break;
            }
            case sigma::graphics::shader_type::tessellation_control: {
                ctx.add_macro_definition("SIGMA_ENGINE_TESSELLATION_CONTROL_SHADER");
                break;
            }
            case sigma::graphics::shader_type::tessellation_evaluation: {
                ctx.add_macro_definition("SIGMA_ENGINE_TESSELLATION_EVALUATION_SHADER");
                break;
            }
            case sigma::graphics::shader_type::geometry: {
                ctx.add_macro_definition("SIGMA_ENGINE_GEOMETRY_SHADER");
                break;
            }
            case sigma::graphics::shader_type::fragment: {
                ctx.add_macro_definition("SIGMA_ENGINE_FRAGMENT_SHADER");
                break;
            }
            case sigma::graphics::shader_type::header: {
                break;
            }
            }

            for (const auto& path : package_settings.source_directories) {
                ctx.add_include_path(path.c_str());
                ctx.add_sysinclude_path(path.c_str());
            }

            auto first = ctx.begin();
            auto last = ctx.end();

            while (first != last) {
                shader.source += (*first).get_value().c_str();
                ++first;
            }

            // Copy all source files that where included while preprocessing to the dependencies list.
            // std::copy(ctx.get_hooks().source_files.begin() + 1, ctx.get_hooks().source_files.end(), std::back_inserter(dependencies));

            glslang::TShader glsl_shader{ shader_types_map.at(shader.type) };
            const char* src = shader.source.c_str();
            glsl_shader.setStrings(&src, 1);

            if (!glsl_shader.parse(&DefaultTBuiltInResource, 450, true, EShMsgDefault)) {
                std::string log_message = glsl_shader.getInfoLog();

                std::regex error_regex("error\\s*:\\s*(\\d+):(\\d+):(.*)", std::regex_constants::icase);
                std::smatch res;

                const auto begin_iterator = std::sregex_iterator(log_message.cbegin(), log_message.cend(), error_regex);
                const auto end_iterator = std::sregex_iterator();

                for (auto it = begin_iterator; it != end_iterator; ++it) {
                    auto match = *it;
                    std::ostringstream ss;
                    // TODO gather more than just one error before throwing
                    // TODO create a compile error exception class
                    ss << ctx.get_hooks().source_files[std::stoi(match[1])].string() << "(" << match[2] << "): error : " << match[3] << '\n'; // TODO remove MSVC output
                    throw std::runtime_error{ ss.str() };
                }
            }

            shader_database.insert(rid, shader, true);
        }

    private:
        ContextType& context_;
    };
}
}

#endif // SIGMA_TOOLS_CUBEMAP_LOADER_HPP
