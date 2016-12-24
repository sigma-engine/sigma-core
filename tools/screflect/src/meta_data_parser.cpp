#include <meta_data_parser.hpp>

#include <iomanip>
#include <sstream>

template <typename PositionT>
inline std::ostream& operator<<(std::ostream& stream, boost::wave::cpplexer::lex_token<PositionT> const& t)
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

translation_unit meta_data_parser::parse_translation_unit(std::vector<token_type>& tokens)
{
    translation_unit tu;
    auto end = parse_translation_unit(tu, tokens.begin(), tokens.end());
    tokens.erase(tokens.begin(), end);
    return tu;
}

const token_type& meta_data_parser::expect(boost::wave::token_id type, meta_data_parser::iterator it)
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

const token_type& meta_data_parser::expect(boost::wave::token_id type, std::string value, meta_data_parser::iterator it)
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

meta_data_parser::iterator meta_data_parser::parse_translation_unit(translation_unit& tu, meta_data_parser::iterator it, meta_data_parser::iterator end)
{
    while (it < end) {
        it = parse_namespace_enum_struct_class(tu, it, end);
        ++it;
    }
    return it;
}

meta_data_parser::iterator meta_data_parser::parse_namespace(namespace_delecration& nspace, meta_data_parser::iterator it, meta_data_parser::iterator end)
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

meta_data_parser::iterator meta_data_parser::parse_enum(enum_delecration& eenum, meta_data_parser::iterator it, meta_data_parser::iterator end)
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

meta_data_parser::iterator meta_data_parser::parse_class(class_delecration& klass, meta_data_parser::iterator it, meta_data_parser::iterator end)
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

meta_data_parser::iterator meta_data_parser::parse_base_type(meta_data_parser::iterator it, meta_data_parser::iterator end)
{
    if (*it == boost::wave::T_PUBLIC || *it == boost::wave::T_PRIVATE || *it == boost::wave::T_PROTECTED || *it == boost::wave::T_VIRTUAL) {
        ++it;
    }
    std::string type_name;
    return parse_qualified_identifier(type_name, it, end);
}

meta_data_parser::iterator meta_data_parser::parse_property(property_delecration& property, meta_data_parser::iterator it, meta_data_parser::iterator end)
{
    expect(boost::wave::T_IDENTIFIER, "RPROPERTY", it++);
    expect(boost::wave::T_LEFTPAREN, it++);
    // TODO fill out
    expect(boost::wave::T_RIGHTPAREN, it++);
    it = parse_type(property.type, it, end);
    property.name = expect(boost::wave::T_IDENTIFIER, it++).get_value().c_str();
    return std::find(it, end, boost::wave::T_SEMICOLON);
}

meta_data_parser::iterator meta_data_parser::parse_qualified_identifier(std::string& type_name, meta_data_parser::iterator it, meta_data_parser::iterator end)
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
