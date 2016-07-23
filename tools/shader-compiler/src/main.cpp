#include <fstream>
#include <iostream>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/program_options.hpp>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

#include <sigmafive/graphics/shader.hpp>
#include <sigmafive/util/compile_time_hash.hpp>
#include <sigmafive/util/filesystem.hpp>

namespace po = boost::program_options;

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
        auto line = "#line " + std::to_string(position.get_line()) + " " + std::to_string(std::distance(source_files.begin(), source_files.end())) + "\n";
        pending.push_back(boost::wave::cpplexer::lex_token<>(boost::wave::T_PP_LINE, line.c_str(), position));
        if (it == source_files.end())
            source_files.push_back(position.get_file().c_str());

        return true;
    }
};

int main(int argc, char const* argv[])
{
    po::options_description global_options("Options");
    global_options.add_options()("help,h", "Show this help message")("output,o", po::value<boost::filesystem::path>()->default_value(boost::filesystem::current_path()), "output directory")("input-files", po::value<std::vector<boost::filesystem::path> >(), "input shaders files");

    po::positional_options_description positional_options;
    positional_options.add("input-files", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(global_options).positional(positional_options).run(), vm);

    if (vm.count("help")) {
        std::cout << global_options << std::endl;
        return -1;
    }

    if (vm.count("input-files") <= 0) {
        std::cerr << "shader-compiler: fatal error: no input files." << std::endl;
        return -1;
    }

    auto outputdir = vm["output"].as<boost::filesystem::path>() / "opengl";
    boost::filesystem::create_directories(outputdir);

    for (auto file_path :
        vm["input-files"].as<std::vector<boost::filesystem::path> >()) {
        file_path = boost::filesystem::absolute(file_path);
        if (sigmafive::util::directory_contains_file(boost::filesystem::current_path(), file_path)) {
            if (boost::filesystem::exists(file_path)) {
                boost::wave::util::file_position_type current_position;
                try {
                    std::ifstream source_file{ file_path.string() };

                    if (!source_file.is_open()) {
                        std::cerr << "could not open file '" << file_path << "'!";
                    } else {
                        source_file.unsetf(std::ios::skipws);
                        std::string input_source(std::istreambuf_iterator<char>(source_file.rdbuf()), std::istreambuf_iterator<char>());

                        typedef boost::wave::cpplexer::lex_token<> token_type;
                        typedef boost::wave::cpplexer::lex_iterator<token_type> lex_iterator_type;
                        typedef boost::wave::context<std::string::iterator, lex_iterator_type, boost::wave::iteration_context_policies::load_file_to_string, glsl_preprocessing_hooks> context_type;

                        auto file_name = file_path.string();
                        context_type ctx(input_source.begin(), input_source.end(), file_name.c_str());

                        context_type::iterator_type first = ctx.begin();
                        context_type::iterator_type last = ctx.end();

                        std::string output_source;
                        while (first != last) {
                            current_position = (*first).get_position();
                            output_source += (*first).get_value().c_str();
                            ++first;
                        }

                        auto ext = file_path.extension();
                        if (ext == ".vert") {
                            sigmafive::graphics::shader shader{ sigmafive::graphics::shader_type::vertex, output_source };

                            sigmafive::resource::development_identifier rid("vertex", file_path);
                            auto final_path = outputdir / std::to_string(rid.value());

                            std::ofstream stream{ final_path.string(), std::ios::binary | std::ios::out };
                            boost::archive::binary_oarchive oa(stream);

                            oa << shader;
                        } else if (ext == ".frag") {
                            sigmafive::graphics::shader shader{ sigmafive::graphics::shader_type::fragment, output_source };

                            sigmafive::resource::development_identifier rid("fragment", file_path);
                            auto final_path = outputdir / std::to_string(rid.value());

                            std::ofstream stream{ final_path.string(), std::ios::binary | std::ios::out };
                            boost::archive::binary_oarchive oa(stream);

                            oa << shader;
                        } else {
                            std::cerr << "shader-compiler: error: " << ext << " not supported." << std::endl;
                        }

                        // TODO validate the shader here.
                    }
                } catch (boost::wave::cpp_exception const& e) {
                    std::cerr << e.file_name() << "(" << e.line_no() << "): " << e.description() << std::endl;
                    return 2;
                } catch (std::exception const& e) {
                    std::cerr << current_position.get_file() << "(" << current_position.get_line() << "): "
                              << "exception caught: " << e.what() << std::endl;
                    return 3;
                } catch (...) {
                    std::cerr << current_position.get_file() << "(" << current_position.get_line() << "): "
                              << "unexpected exception caught." << std::endl;
                    return 4;
                }
            } else {
                std::cerr << "shader-compiler: error: file '" << file_path << "' does not exist!" << std::endl;
            }
        } else {
            std::cerr << "shader-compiler: error: file '" << file_path << "' is not contained in '" << boost::filesystem::current_path() << "'!" << std::endl;
        }
    }

    return 0;
}
