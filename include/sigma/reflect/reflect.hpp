#ifndef SIGMA_REFLECT_HPP
#define SIGMA_REFLECT_HPP

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

#endif // SIGMA_REFLECT_HPP
