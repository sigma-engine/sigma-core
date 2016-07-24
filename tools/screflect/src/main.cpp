#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <boost/algorithm/string/join.hpp>
#include <boost/assign.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/util/cpp_macromap_utils.hpp>

typedef boost::wave::cpplexer::lex_token<> token_type;
typedef boost::wave::cpplexer::lex_iterator<token_type> lexer_type;
typedef token_type::position_type position_type;

template <typename PositionT>
inline std::ostream&
operator<<(std::ostream& stream,
    boost::wave::cpplexer::lex_token<PositionT> const& t)
{
    using namespace std;
    using namespace boost::wave;

    token_id id = token_id(t);
    stream << setw(16)
           << left << boost::wave::get_token_name(id) << " ("
           << "#" << setw(3) << BASEID_FROM_TOKEN(id);

    if (ExtTokenTypeMask & id) {
        // this is an extended token id
        if (AltTokenType == (id & ExtTokenOnlyMask)) {
            stream << ", AltTokenType";
        } else if (TriGraphTokenType == (id & ExtTokenOnlyMask)) {
            stream << ", TriGraphTokenType";
        } else if (AltExtTokenType == (id & ExtTokenOnlyMask)) {
            stream << ", AltExtTokenType";
        }
    }

    stream
        << ") at " << t.get_position().get_file() << " ("
        << setw(3) << right << t.get_position().get_line() << "/"
        << setw(2) << right << t.get_position().get_column()
        << "): >";

    typedef typename boost::wave::cpplexer::lex_token<PositionT>::string_type
        string_type;

    string_type const& value = t.get_value();
    for (std::size_t i = 0; i < value.size(); ++i) {
        switch (value[i]) {
        case '\r':
            stream << "\\r";
            break;
        case '\n':
            stream << "\\n";
            break;
        case '\t':
            stream << "\\t";
            break;
        default:
            stream << value[i];
            break;
        }
    }
    stream << "<";

    return stream;
}

bool not_whitespace(const token_type& token)
{
    return !IS_CATEGORY(token, boost::wave::WhiteSpaceTokenType) && !IS_CATEGORY(token, boost::wave::EOLTokenType);
}

template <class Iterator, class T>
Iterator find_end_of_scope(Iterator begin, Iterator end, T begin_scope, T end_scope)
{
    auto it = std::find(begin, end, begin_scope);
    int level = 0;
    do {
        if (*it == begin_scope)
            level++;
        else if (*it == end_scope)
            level--;

        ++it;
    } while (it < end && level > 0);
    return it;
}

struct property_delecration {
    std::string type;
    std::string name;
};

struct enum_delecration {
    std::string name;
    std::vector<std::string> values;
};

struct class_delecration {
    std::string name;
    std::vector<enum_delecration> enums;
    std::vector<class_delecration> classes;
    std::vector<property_delecration> properties;
};

struct namespace_delecration {
    std::string name;
    std::vector<enum_delecration> enums;
    std::vector<class_delecration> classes;
    std::vector<namespace_delecration> namespaces;
};

struct translation_unit {
    std::vector<enum_delecration> enums;
    std::vector<class_delecration> classes;
    std::vector<namespace_delecration> namespaces;
};

class meta_data_parser {
    using iterator = std::vector<token_type>::iterator;

public:
    translation_unit parse_translation_unit(std::vector<token_type>& tokens)
    {
        translation_unit tu;
        auto end = parse_translation_unit(tu, tokens.begin(), tokens.end());
        tokens.erase(tokens.begin(), end);
        return tu;
    }

private:
    const token_type& expect(boost::wave::token_id type, iterator it)
    {
        if (*it != type) {
            auto expect_string = (type != boost::wave::T_IDENTIFIER) ? boost::wave::get_token_value(type) : boost::wave::get_token_name(type);
            std::stringstream ss;
            auto pos = it->get_position();
            ss << pos.get_file() << ":" << pos.get_line() << ":" << pos.get_column()
               << " error: "
               << "expected '" << expect_string << "' but got '" << it->get_value() << "' token" << std::endl;
            throw std::runtime_error(ss.str());
        }
        return *it;
    }

    const token_type& expect(boost::wave::token_id type, std::string value, iterator it)
    {
        if (*it != type || it->get_value().c_str() != value) {
            std::stringstream ss;
            auto pos = it->get_position();
            ss << pos.get_file() << ":" << pos.get_line() << ":" << pos.get_column()
               << " error: "
               << "expected '" << value << "' but got '" << it->get_value() << "' token" << std::endl;
            throw std::runtime_error(ss.str());
        }
        return *it;
    }

    iterator parse_translation_unit(translation_unit& tu, iterator it, iterator end)
    {
        while (it < end) {
            it = parse_namespace_enum_struct_class(tu, it, end);
            ++it;
        }
        return it;
    }

    template <class ParentScope>
    iterator parse_namespace_enum_struct_class(ParentScope& parent, iterator it, iterator end)
    {
        if (*it == boost::wave::T_NAMESPACE) {
            namespace_delecration ns;
            it = parse_namespace(ns, it, end);
            if (!ns.namespaces.empty() || !ns.enums.empty() || !ns.classes.empty())
                parent.namespaces.push_back(std::move(ns));
        } else if (*it == boost::wave::T_IDENTIFIER) {
            it = parse_enum_struct_class(parent, it, end);
        }
        return it;
    }

    template <class ParentScope>
    iterator parse_enum_struct_class(ParentScope& parent, iterator it, iterator end)
    {
        auto id = it->get_value();
        if (id == "RCLASS") {
            class_delecration klass;
            it = parse_class(klass, it, end);
            parent.classes.push_back(std::move(klass));
        } else if (id == "RENUM") {
            enum_delecration eenum;
            it = parse_enum(eenum, it, end);
            parent.enums.push_back(std::move(eenum));
        }
        return it;
    }

    iterator parse_namespace(namespace_delecration& nspace, iterator it, iterator end)
    {
        expect(boost::wave::T_NAMESPACE, it++);

        auto brace = std::find(it, end, boost::wave::T_LEFTBRACE);
        auto colon = std::find(it, end, boost::wave::T_SEMICOLON);
        if (colon < brace) // skip 'using namespace ...;'
            return colon;

        if (*it == boost::wave::T_LEFTBRACE) // skip 'anonymous namespace'
            return find_end_of_scope(it, end, boost::wave::T_LEFTBRACE, boost::wave::T_RIGHTBRACE);

        nspace.name = expect(boost::wave::T_IDENTIFIER, it++).get_value().c_str();

        auto end_of_namespace = find_end_of_scope(it, end, boost::wave::T_LEFTBRACE, boost::wave::T_RIGHTBRACE);

        expect(boost::wave::T_LEFTBRACE, it++);

        while (it < end_of_namespace) {
            it = parse_namespace_enum_struct_class(nspace, it, end);
            ++it;
        }

        expect(boost::wave::T_RIGHTBRACE, it - 1);

        return end_of_namespace;
    }

    iterator parse_enum(enum_delecration& eenum, iterator it, iterator end)
    {
        auto end_of_enum = find_end_of_scope(it, end, boost::wave::T_LEFTBRACE, boost::wave::T_RIGHTBRACE);

        expect(boost::wave::T_IDENTIFIER, "RENUM", it++);

        expect(boost::wave::T_LEFTPAREN, it++);
        // TOOD fill out
        expect(boost::wave::T_RIGHTPAREN, it++);

        it = std::min(std::find(it, end_of_enum, boost::wave::T_COLON),
                 std::find(it, end_of_enum, boost::wave::T_LEFTBRACE))
            - 1;

        eenum.name = expect(boost::wave::T_IDENTIFIER, it++).get_value().c_str();

        /*if (*it == boost::wave::T_COLON) {
            ++it;
            do {
                it = parse_base_type(it, end_of_enum); // TODO do some thing with these
            } while (*it == boost::wave::T_COMMA);
        }*/

        expect(boost::wave::T_LEFTBRACE, it++);

        while (it < end_of_enum && *it == boost::wave::T_IDENTIFIER) {
            eenum.values.push_back(it->get_value().c_str());
            it = std::find(it, end_of_enum - 1, boost::wave::T_COMMA) + 1;
        }

        expect(boost::wave::T_RIGHTBRACE, it - 1);

        return end_of_enum;
    }

    iterator parse_enum_value(std::string& value, iterator it, iterator end)
    {
        value = expect(boost::wave::T_IDENTIFIER, it++).get_value().c_str();
        return std::find(it, end, boost::wave::T_COMMA);
    }

    iterator parse_class(class_delecration& klass, iterator it, iterator end)
    {
        auto end_of_class = find_end_of_scope(it, end, boost::wave::T_LEFTBRACE, boost::wave::T_RIGHTBRACE);

        expect(boost::wave::T_IDENTIFIER, "RCLASS", it++);

        expect(boost::wave::T_LEFTPAREN, it++);
        // TOOD fill out
        expect(boost::wave::T_RIGHTPAREN, it++);

        it = std::min(std::find(it, end_of_class, boost::wave::T_COLON),
                 std::find(it, end_of_class, boost::wave::T_LEFTBRACE))
            - 1;

        klass.name = expect(boost::wave::T_IDENTIFIER, it++).get_value().c_str();

        if (*it == boost::wave::T_COLON) {
            ++it;
            do {
                it = parse_base_type(it, end_of_class); // TODO do some thing with these
            } while (*it == boost::wave::T_COMMA);
        }

        expect(boost::wave::T_LEFTBRACE, it++);

        while (it < end_of_class) {
            it = parse_enum_struct_class(klass, it, end);
            if (*it == boost::wave::T_IDENTIFIER) {
                auto id = it->get_value();
                if (id == "RPROPERTY") {
                    property_delecration property;
                    it = parse_property(property, it, end);
                    klass.properties.push_back(property);
                }
            }
            ++it;
        }

        expect(boost::wave::T_RIGHTBRACE, it - 1);

        return end_of_class;
    }

    iterator parse_base_type(iterator it, iterator end)
    {
        if (*it == boost::wave::T_PUBLIC || *it == boost::wave::T_PRIVATE || *it == boost::wave::T_PROTECTED || *it == boost::wave::T_VIRTUAL) {
            ++it;
        }
        std::string type_name;
        return parse_qualified_identifier(type_name, it, end);
    }

    iterator parse_property(property_delecration& property, iterator it, iterator end)
    {
        expect(boost::wave::T_IDENTIFIER, "RPROPERTY", it++);
        expect(boost::wave::T_LEFTPAREN, it++);
        // TODO fill out
        expect(boost::wave::T_RIGHTPAREN, it++);
        it = parse_type(property.type, it, end);
        property.name = expect(boost::wave::T_IDENTIFIER, it++).get_value().c_str();
        return std::find(it, end, boost::wave::T_SEMICOLON);
    }

    iterator parse_type(std::string& type, iterator it, iterator end)
    {
        it = parse_qualified_identifier(type, it, end);
        while (*it == boost::wave::T_AND || *it == boost::wave::T_STAR) {
            type += it->get_value().c_str();
            ++it;
        }
        return it;
    }

    iterator parse_qualified_identifier(std::string& type_name, iterator it, iterator end)
    {
        do {
            type_name += it->get_value().c_str();
            expect(boost::wave::T_IDENTIFIER, it++);
            if (*it != boost::wave::T_COLON_COLON)
                break;
            type_name += "::";
            ++it;
        } while (it < end);
        return it;
    }
};

void print(const enum_delecration& parent, std::size_t level = 0)
{
    std::string indent(4 * level, ' ');
    std::cout << indent << "enum " << parent.name << std::endl;
    std::cout << indent << "{" << std::endl;
    std::cout << boost::join(parent.values | boost::adaptors::transformed([indent](const auto& value) { return indent + std::string(4, ' ') + value; }), ",\n") << std::endl;
    std::cout << indent << "}" << std::endl;
}

void print(const property_delecration& parent, std::size_t level = 0)
{
    std::string indent(4 * level, ' ');
    std::cout << indent << parent.type << " " << parent.name << ";" << std::endl;
}

void print(const class_delecration& parent, std::size_t level = 0)
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

void print(const namespace_delecration& parent, std::size_t level = 0)
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

void print(const translation_unit& parent, std::size_t level = 0)
{
    for (const auto& e : parent.enums)
        print(e, level);
    for (const auto& kl : parent.classes)
        print(kl, level);
    for (const auto& ns : parent.namespaces)
        print(ns, level);
}

class meta_data_indexer {
    struct block {
        int level;
        std::string name;
        bool keep;
    };

public:
    bool index(boost::filesystem::path path)
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
                    print(tu);
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

private:
};

int main(int argc, char const* argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    meta_data_indexer indexer;
    for (int i = 1; i < argc; ++i) {
        try {
            indexer.index(argv[i]);
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << "ms" << std::endl;
    return 0;
}
