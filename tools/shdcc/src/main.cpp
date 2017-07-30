#include <sigma/graphics/shader.hpp>
#include <sigma/util/filesystem.hpp>

#include <glslang/MachineIndependent/reflection.h>
#include <glslang/Public/ShaderLang.h>

#include <json/json.h>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>

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

typedef boost::wave::cpplexer::lex_token<> token_type;
typedef boost::wave::cpplexer::lex_iterator<token_type> lex_iterator_type;
typedef boost::wave::context<std::string::iterator, lex_iterator_type, boost::wave::iteration_context_policies::load_file_to_string, glsl_preprocessing_hooks> context_type;

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

int main(int argc, char const* argv[])
{
    std::unordered_map<std::string, std::tuple<sigma::graphics::shader_type, EShLanguage, std::string>> extention_type_map = {
        { ".vert", { sigma::graphics::shader_type::vertex, EShLangVertex, "vertex" } },
        { ".frag", { sigma::graphics::shader_type::fragment, EShLangFragment, "fragment" } },
        { ".geom", { sigma::graphics::shader_type::geometry, EShLangGeometry, "geometry" } },
        { ".glsl", { sigma::graphics::shader_type::header, EShLangCount, "header" } }
    };

    boost::program_options::options_description global{ "options" };
    boost::program_options::positional_options_description positional;

    // clang-format off
    global.add_options()
    ("source-directory,H", boost::program_options::value<std::string>()->default_value(boost::filesystem::current_path().string()), "The top level package directory.")
    ("build-directory,B", boost::program_options::value<std::string>()->default_value(boost::filesystem::current_path().string()), "The top level project build directory.")
    ("dependency,M", "List dependencies of the shader conversion.")
    ("include-directory,I", boost::program_options::value<std::vector<std::string>>()->default_value(std::vector<std::string>{}, ""), "Extra include directories")
    ("source-file,c", boost::program_options::value<std::string>()->required(), "The shader file to convert.");
    // clang-format on

    positional.add("source-file", -1);

    boost::wave::util::file_position_type current_position;
    try {
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(global).positional(positional).run(), vm);
        boost::program_options::notify(vm);

        auto source_file = boost::filesystem::canonical(vm["source-file"].as<std::string>());
        if (!boost::filesystem::exists(source_file))
            throw std::runtime_error("'" + source_file.string() + "' does not exist.");

        auto extension = boost::to_lower_copy(source_file.extension().string());
        auto shader_type_info = extention_type_map.find(extension);
        if (shader_type_info == extention_type_map.end())
            throw std::runtime_error("Unknown file format with extention '" + extension + "'");

        auto source_directory = boost::filesystem::canonical(vm["source-directory"].as<std::string>());
        auto rid = std::get<2>(shader_type_info->second) / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
        auto output_directory = boost::filesystem::canonical(vm["build-directory"].as<std::string>()) / "data";
        auto output_file = output_directory / rid;
        auto uniforms_file = output_file;
        uniforms_file += ".uniforms";

        std::ifstream source{ source_file.string() };
        std::string source_code{ std::istreambuf_iterator<char>{ source.rdbuf() }, std::istreambuf_iterator<char>{} };

        auto file_name = source_file.string();
        context_type ctx{ source_code.begin(), source_code.end(), file_name.c_str() };
        ctx.get_hooks().source_files.push_back(source_file);

        ctx.add_macro_definition("SIGMA_ENGINE_SHADER");
        switch (std::get<0>(shader_type_info->second)) {
        case sigma::graphics::shader_type::vertex: {
            ctx.add_macro_definition("SIGMA_ENGINE_VERTEX_SHADER");
            break;
        }
        case sigma::graphics::shader_type::fragment: {
            ctx.add_macro_definition("SIGMA_ENGINE_FRAGMENT_SHADER");
            break;
        }
        case sigma::graphics::shader_type::geometry: {
            ctx.add_macro_definition("SIGMA_ENGINE_GEOMETRY_SHADER");
            break;
        }
        case sigma::graphics::shader_type::header: {
            break;
        }
        }

        for (auto path : vm["include-directory"].as<std::vector<std::string>>()) {
            boost::trim(path);
            if (path.size() > 0) {
                ctx.add_include_path(path.c_str());
                ctx.add_sysinclude_path(path.c_str());
            }
        }

        auto first = ctx.begin();
        auto last = ctx.end();

        std::string preprocessed_source_code;
        while (first != last) {
            current_position = (*first).get_position();
            preprocessed_source_code += (*first).get_value().c_str();
            ++first;
        }

        if (vm.count("dependency")) {
            boost::filesystem::path dependency_path = output_file;
            dependency_path.replace_extension(source_file.extension().string() + ".dependency");
            std::ofstream dep{ dependency_path.string() };

            std::regex re{ "[^a-zA-Z0-9]" };
            dep << "set(" << std::regex_replace(rid.string(), re, "_") << "_DEPENDS\n";
            for (int i = 1; i < ctx.get_hooks().source_files.size(); ++i)
                dep << ctx.get_hooks().source_files[i] << '\n';
            dep << ")\n";

            return 0;
        }

        sigma::graphics::shader shader;
        shader.type = std::get<0>(shader_type_info->second);
        shader.source = preprocessed_source_code;

        glslang::process glslang_proc;
        glslang::TShader glsl_shader{ std::get<1>(shader_type_info->second) };
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
                std::cerr << ctx.get_hooks().source_files[std::stoi(match[1])].string() << "(" << match[2] << "): error : " << match[3] << '\n'; // MSVC output
            }
            return -1;
        }

        auto output_folder = output_file.parent_path();
        if (!boost::filesystem::exists(output_folder))
            boost::filesystem::create_directories(output_folder);

        std::ofstream stream{ output_file.string(), std::ios::binary };
        boost::archive::binary_oarchive oa(stream);
        oa << shader;
    } catch (const boost::program_options::error& e) {
        std::cerr << "shdrcc\nerror: " << e.what() << '\n';
        std::cerr << global << '\n';
        return -1;
    } catch (const boost::wave::cpp_exception& e) {
        std::cerr << e.file_name() << "(" << e.line_no() << "): " << e.description() << '\n';
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "shdrcc\nerror: " << e.what() << '\n';
        return -1;
    }
    return 0;
}
