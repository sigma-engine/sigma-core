#ifndef screflect_REFLECT_HPP
#define screflect_REFLECT_HPP

#define RCLASS(...)
#define RENUM(...)
#define RPROPERTY(...)
#define RFUNCTION(...)

#include <string>
#include <vector>

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

/*namespace reflect {
struct r_class {
    const char* name;
};

struct r_enum {
    const char* name;
};

struct r_property {
    const char* name;
};

struct r_function {
    const char* name;
};

namespace detail {
    struct r_module {
        const char* name;
        std::vector<r_class> classes;
        std::vector<r_enum> enums;
        std::vector<r_property> properties;
        std::vector<r_function> functions;
    };
}
}*/

#endif //screflect_REFLECT_HPP
