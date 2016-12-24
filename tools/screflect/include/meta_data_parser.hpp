#ifndef SIGMA_ENGINE_META_DATA_PARSER
#define SIGMA_ENGINE_META_DATA_PARSER

#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <sigma/reflect/reflect.hpp>

typedef boost::wave::cpplexer::lex_token<> token_type;
typedef boost::wave::cpplexer::lex_iterator<token_type> lexer_type;
typedef token_type::position_type position_type;

class meta_data_parser {
    using iterator = std::vector<token_type>::iterator;

public:
    translation_unit parse_translation_unit(std::vector<token_type>& tokens);

private:
    const token_type& expect(boost::wave::token_id type, iterator it);

    const token_type& expect(boost::wave::token_id type, std::string value, iterator it);

    iterator parse_translation_unit(translation_unit& tu, iterator it, iterator end);

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

    iterator parse_namespace(namespace_delecration& nspace, iterator it, iterator end);

    iterator parse_enum(enum_delecration& eenum, iterator it, iterator end);

    iterator parse_enum_value(std::string& value, iterator it, iterator end)
    {
        value = expect(boost::wave::T_IDENTIFIER, it++).get_value().c_str();
        return std::find(it, end, boost::wave::T_COMMA);
    }

    iterator parse_class(class_delecration& klass, iterator it, iterator end);

    iterator parse_base_type(iterator it, iterator end);

    iterator parse_property(property_delecration& property, iterator it, iterator end);

    iterator parse_type(std::string& type, iterator it, iterator end)
    {
        it = parse_qualified_identifier(type, it, end);
        while (*it == boost::wave::T_AND || *it == boost::wave::T_STAR) {
            type += it->get_value().c_str();
            ++it;
        }
        return it;
    }

    iterator parse_qualified_identifier(std::string& type_name, iterator it, iterator end);
};

#endif // SIGMA_ENGINE_META_DATA_PARSER
