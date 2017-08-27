#ifndef SIGMA_TOOLS_EFFCC_SHADER_COMPILE_HPP
#define SIGMA_TOOLS_EFFCC_SHADER_COMPILE_HPP

#include <sigma/graphics/shader.hpp>

#include <glslang/MachineIndependent/reflection.h>
#include <glslang/Public/ShaderLang.h>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

#include <regex>
#include <sstream>

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

namespace glslang {
extern const TBuiltInResource DefaultTBuiltInResource;
struct process {
    process()
    {
        glslang::InitializeProcess();
    }
    ~process()
    {
        glslang::FinalizeProcess();
    }
};
}

typedef boost::wave::cpplexer::lex_token<> token_type;
typedef boost::wave::cpplexer::lex_iterator<token_type> lex_iterator_type;
typedef boost::wave::context<std::string::iterator, lex_iterator_type, boost::wave::iteration_context_policies::load_file_to_string, glsl_preprocessing_hooks> context_type;

auto compile_shader(std::vector<boost::filesystem::path>& dependencies, const std::vector<boost::filesystem::path>& include_directories, sigma::graphics::shader_type type, const boost::filesystem::path& source_file, std::string source_code)
{
    const static std::unordered_map<sigma::graphics::shader_type, EShLanguage> shader_types_map = {
        { sigma::graphics::shader_type::vertex, EShLangVertex },
        { sigma::graphics::shader_type::tessellation_control, EShLangTessControl },
        { sigma::graphics::shader_type::tessellation_evaluation, EShLangTessEvaluation },
        { sigma::graphics::shader_type::geometry, EShLangGeometry },
        { sigma::graphics::shader_type::fragment, EShLangFragment }
    };

    sigma::graphics::shader shader;
    shader.type = type;

    context_type ctx{ source_code.begin(), source_code.end(), source_file.c_str() };
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

    for (const auto& path : include_directories) {
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
    std::copy(ctx.get_hooks().source_files.begin() + 1, ctx.get_hooks().source_files.end(), std::back_inserter(dependencies));

    glslang::TShader glsl_shader{ shader_types_map.at(shader.type) };
    const char* src = shader.source.c_str();
    glsl_shader.setStrings(&src, 1);

    if (!glsl_shader.parse(&glslang::DefaultTBuiltInResource, 450, true, EShMsgDefault)) {
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

    return shader; //std::make_pair(shader, glsl_shader);
}

#endif // SIGMA_TOOLS_EFFCC_SHADER_COMPILE_HPP
