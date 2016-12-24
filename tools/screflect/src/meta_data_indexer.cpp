#include <meta_data_indexer.hpp>

#include <iostream>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/wave.hpp>

#include <meta_data_parser.hpp>

struct block {
    int level;
    std::string name;
    bool keep;
};

void print(const enum_delecration& parent, std::size_t level)
{
    std::string indent(4 * level, ' ');
    std::cout << indent << "enum " << parent.name << std::endl;
    std::cout << indent << "{" << std::endl;
    std::cout << boost::join(parent.values | boost::adaptors::transformed([indent](const auto& value) { return indent + std::string(4, ' ') + value; }), ",\n") << std::endl;
    std::cout << indent << "}" << std::endl;
}

void print(const property_delecration& parent, std::size_t level)
{
    std::string indent(4 * level, ' ');
    std::cout << indent << parent.type << " " << parent.name << ";" << std::endl;
}

void print(const class_delecration& parent, std::size_t level)
{
    std::string indent(4 * level, ' ');
    std::cout << indent << "class " << parent.name << std::endl;
    std::cout << indent << "{" << std::endl;
    for (const auto& e : parent.enums)
        print(e, level + 1);
    for (const auto& kl : parent.classes)
        print(kl, level + 1);
    for (const auto& p : parent.properties)
        print(p, level + 1);
    std::cout << indent << "}" << std::endl;
}

void print(const namespace_delecration& parent, std::size_t level)
{
    std::string indent(4 * level, ' ');
    std::cout << indent << "namespace " << parent.name << std::endl;
    std::cout << indent << "{" << std::endl;
    for (const auto& e : parent.enums)
        print(e, level + 1);
    for (const auto& kl : parent.classes)
        print(kl, level + 1);
    for (const auto& ns : parent.namespaces)
        print(ns);
    std::cout << indent << "}" << std::endl;
}

void print(const translation_unit& parent, std::size_t level)
{
    for (const auto& e : parent.enums)
        print(e, level);
    for (const auto& kl : parent.classes)
        print(kl, level);
    for (const auto& ns : parent.namespaces)
        print(ns, level);
}

template <typename T>
void merge_by_name(T& dest, const T& src)
{
    for (const auto& src_elem : src) {
        auto it = std::find_if(std::begin(dest), std::end(dest), [&src_elem](auto dest_elem) {
            return src_elem.name == dest_elem.name;
        });
        if (it != std::end(dest))
            merge(*it, src_elem);
        else
            dest.push_back(src_elem);
    }
}

void merge(enum_delecration& dest, const enum_delecration& src)
{
    // TODO error more than one definition of enum
}

void merge(class_delecration& dest, const class_delecration& src)
{
    // TODO error more than one definition of class
}

void merge(namespace_delecration& dest, const namespace_delecration& src)
{
    merge_by_name(dest.enums, src.enums);
    merge_by_name(dest.classes, src.classes);
    merge_by_name(dest.namespaces, src.namespaces);
}

void merge(translation_unit& dest, const translation_unit& src)
{
    merge_by_name(dest.enums, src.enums);
    merge_by_name(dest.classes, src.classes);
    merge_by_name(dest.namespaces, src.namespaces);
}

bool not_whitespace(const token_type& token)
{
    return !IS_CATEGORY(token, boost::wave::WhiteSpaceTokenType) && !IS_CATEGORY(token, boost::wave::EOLTokenType);
}

bool meta_data_indexer::index(boost::filesystem::path path)
{
    meta_data_parser parser;

    std::ifstream instream(path.string());
    if (instream.is_open()) {
        std::vector<token_type> input_tokens;
        try {
            instream.unsetf(std::ios::skipws);
            std::string instr = std::string(std::istreambuf_iterator<char>(instream.rdbuf()), std::istreambuf_iterator<char>());

            position_type pos(path.string().c_str());
            lexer_type token_begin = lexer_type(instr.begin(), instr.end(), pos, boost::wave::language_support(boost::wave::support_cpp | boost::wave::support_option_long_long));
            lexer_type token_end = lexer_type();

            std::copy_if(token_begin, token_end, std::back_inserter(input_tokens), not_whitespace);
        } catch (boost::wave::cpplexer::lexing_exception& e) {
            std::cerr << e.file_name() << ":" << e.line_no() << ":" << e.column_no() << " " << e.description() << std::endl;
            return false;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            return false;
        } catch (...) {
            std::cerr << "unexpected exception caught." << std::endl;
            return false;
        }

        if (!input_tokens.empty()) {
            try {
                translation_unit tu = parser.parse_translation_unit(input_tokens);
                merge(global_translation_unit_, tu);
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
                return false;
            }
        }
    } else {
        std::cerr << "Could not open input file: " << path << std::endl;
        return false;
    }
    return true;
}

void meta_data_indexer::print()
{
    ::print(global_translation_unit_);
}
