#include <compiler_util.hpp>
#include <shader_compiler.hpp>
#include <shader_validation.hpp>

#include <sigma/graphics/shader.hpp>

#pragma warning(push, 0)
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#pragma warning(pop)

#include <boost/archive/binary_oarchive.hpp>

#include <string>
#include <unordered_map>

namespace sigma {
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
		auto size = source_files.size();
        std::size_t file_index = it - source_files.end();
        if (file_index >= size)
			file_index = size;
			source_files.push_back(position.get_file().c_str());

        auto line = "#line " + std::to_string(position.get_line()) + " " + std::to_string(file_index) + "\n";
        pending.push_back(boost::wave::cpplexer::lex_token<>(boost::wave::T_PP_LINE, line.c_str(), position));

        return true;
    }
};

typedef boost::wave::cpplexer::lex_token<> token_type;
typedef boost::wave::cpplexer::lex_iterator<token_type> lex_iterator_type;
typedef boost::wave::context<std::string::iterator, lex_iterator_type, boost::wave::iteration_context_policies::load_file_to_string, glsl_preprocessing_hooks> context_type;

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
			std::string input_source{ std::istreambuf_iterator<char>(source_file.rdbuf()), std::istreambuf_iterator<char>() };

            sigma::graphics::shader_data shader;
            shader.type = ext_to_type[file_path.extension().string()];
            sigma::resource::development_identifier rid;

            auto file_name = file_path.string();
			context_type ctx{ input_source.begin(), input_source.end(), file_name.c_str() };

            ctx.add_macro_definition("SIGMA_ENGINE_SHADER");

            switch (shader.type) {
            case graphics::shader_type::vertex: {
                ctx.add_macro_definition("SIGMA_ENGINE_VERTEX_SHADER");
                rid = sigma::resource::development_identifier{ "vertex", file_path };
                break;
            }
            case graphics::shader_type::fragment: {
                ctx.add_macro_definition("SIGMA_ENGINE_FRAGMENT_SHADER");
                rid = sigma::resource::development_identifier{ "fragment", file_path };
                break;
            }
            case graphics::shader_type::geometry: {
                ctx.add_macro_definition("SIGMA_ENGINE_GEOMETRY_SHADER");
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

            auto first = ctx.begin();
            auto last = ctx.end();

            while (first != last) {
                current_position = (*first).get_position();
                shader.source += (*first).get_value().c_str();
                ++first;
            }

            if (validate_shader(shader, ctx.get_hooks().source_files)) {
                auto final_path = outputdir / std::to_string(rid.value());
                std::ofstream stream{ final_path.string(), std::ios::binary | std::ios::out };
				boost::archive::binary_oarchive oa{ stream };
                oa << shader;
                touch_stamp_file(outputdir, file_path);
            } else {
                all_good = false;
            }

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
    return all_good;
}
}
