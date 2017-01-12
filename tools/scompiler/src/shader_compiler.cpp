#include <compiler_util.hpp>
#include <shader_compiler.hpp>

#include <sigma/graphics/shader.hpp>

#include "../glslang/glslang/Public/ShaderLang.h"

#pragma warning(push, 0)
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#pragma warning(pop)

#include <boost/archive/binary_oarchive.hpp>

#include <unordered_map>
#include <regex>
#include <string>

namespace sigma {
const TBuiltInResource default_resources = {
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

class glsl_preprocessing_hooks : public boost::wave::context_policies::default_preprocessing_hooks {
public:
    std::vector<std::string> source_files;

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
		std::size_t file_index = 0;
		if (it != source_files.end()) {
			file_index = std::distance(it, source_files.end());
		}
		else {
			file_index = source_files.size();
			source_files.push_back(position.get_file().c_str());
		}

        auto line = "#line " + std::to_string(position.get_line()) + " " + std::to_string(file_index) + "\n";
        pending.push_back(boost::wave::cpplexer::lex_token<>(boost::wave::T_PP_LINE, line.c_str(), position));

        return true;
    }
};

static std::unordered_map<std::string, graphics::shader_type> ext_to_type = {
    { ".vert", graphics::shader_type::vertex },
    { ".frag", graphics::shader_type::fragment },
    { ".geom", graphics::shader_type::geometry },
    { ".glsl", graphics::shader_type::header }
};

bool is_shader(boost::filesystem::path file)
{
    return ext_to_type.count(file.extension().string()) > 0;
}

bool compile_shaders(boost::filesystem::path outputdir, std::vector<boost::filesystem::path> include_dirs, std::vector<boost::filesystem::path> shaders)
{
    glslang::InitializeProcess();
    bool all_good = true;
    for (const auto& file_path : shaders) {
        // TODO account for #include changes
        // TODO just skip .glsl for now
        if (!resource_has_changes(outputdir, file_path) || file_path.extension() == ".glsl")
            continue;

        std::cout << file_path.filename().string() << std::endl;
        boost::wave::util::file_position_type current_position;
        try {
            std::ifstream source_file{ file_path.string() };

            source_file.unsetf(std::ios::skipws);
            std::string input_source(std::istreambuf_iterator<char>(source_file.rdbuf()), std::istreambuf_iterator<char>());

            typedef boost::wave::cpplexer::lex_token<> token_type;
            typedef boost::wave::cpplexer::lex_iterator<token_type> lex_iterator_type;
            typedef boost::wave::context<std::string::iterator, lex_iterator_type, boost::wave::iteration_context_policies::load_file_to_string, glsl_preprocessing_hooks> context_type;

            auto file_name = file_path.string();
            context_type ctx(input_source.begin(), input_source.end(), file_name.c_str());

            ctx.add_macro_definition("SIGMA_ENGINE_SHADER");

            EShLanguage glslLangType;
            sigma::graphics::shader_data shader;
            shader.type = ext_to_type[file_path.extension().string()];
            sigma::resource::development_identifier rid;

            switch (shader.type) {
            case graphics::shader_type::vertex: {
                ctx.add_macro_definition("SIGMA_ENGINE_VERTEX_SHADER");
                rid = sigma::resource::development_identifier{ "vertex", file_path };
                glslLangType = EShLangVertex;
                break;
            }
            case graphics::shader_type::fragment: {
                ctx.add_macro_definition("SIGMA_ENGINE_FRAGMENT_SHADER");
                rid = sigma::resource::development_identifier{ "fragment", file_path };
                glslLangType = EShLangFragment;
                break;
            }
            case graphics::shader_type::geometry: {
                ctx.add_macro_definition("SIGMA_ENGINE_GEOMETRY_SHADER");
                glslLangType = EShLangGeometry;
                rid = sigma::resource::development_identifier{ "geometry", file_path };
                break;
            }
            case graphics::shader_type::header: {
                rid = sigma::resource::development_identifier{ "header", file_path };
                break;
            }
            }

            for (const auto& include_path : include_dirs) {
                auto dir = boost::filesystem::absolute(include_path);
                ctx.add_include_path(dir.string().c_str());
                ctx.add_sysinclude_path(dir.string().c_str());
            }

            context_type::iterator_type first = ctx.begin();
            context_type::iterator_type last = ctx.end();

            while (first != last) {
                current_position = (*first).get_position();
                shader.source += (*first).get_value().c_str();
                ++first;
            }

            glslang::TShader glslShader{ glslLangType };
            const char* src = shader.source.c_str();
            glslShader.setStrings(&src, 1);

			if (!glslShader.parse(&default_resources, 330, true, EShMsgDefault)) {
				

				std::string error_message = glslShader.getInfoLog();
				//std::cout << "<<< " << error_message << " >>>" << std::endl;

				std::regex error_regex("error\\s*:\\s*(\\d+):(\\d+):(.*)", std::regex_constants::icase);
				std::smatch res;

				const auto begin_iterator = std::sregex_iterator(error_message.cbegin(), error_message.cend(), error_regex);
				const auto end_iterator = std::sregex_iterator();

				for (auto it = begin_iterator; it != end_iterator; ++it)
				{
					auto match = *it;
					std::cerr << ctx.get_hooks().source_files[std::stoi(match[1])] << "(" << match[2] << "): error :" << match[3] << std::endl; // MSVC output
				}

				all_good = false;
				continue;
			}

		

            auto final_path = outputdir / std::to_string(rid.value());
            std::ofstream stream{ final_path.string(), std::ios::binary | std::ios::out };
            boost::archive::binary_oarchive oa(stream);
            oa << shader;

            touch_stamp_file(outputdir, file_path);
        } catch (boost::wave::cpp_exception const& e) {
            all_good = false;
            std::cerr << e.file_name() << "(" << e.line_no() << "): " << e.description() << std::endl;
        } catch (std::exception const& e) {
            all_good = false;
            std::cerr << current_position.get_file() << "(" << current_position.get_line() << "): "
                      << "exception caught: " << e.what() << std::endl;
        } catch (...) {
            all_good = false;
            std::cerr << current_position.get_file() << "(" << current_position.get_line() << "): "
                      << "unexpected exception caught." << std::endl;
        }
    }
    glslang::FinalizeProcess();
    return all_good;
}
}
