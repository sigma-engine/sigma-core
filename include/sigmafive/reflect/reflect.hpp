#ifndef REFLECTOR_REFLECT_HPP
#define REFLECTOR_REFLECT_HPP

#define RCLASS(...)
#define RENUM(...)
#define RPROPERTY(...)
#define RFUNCTION(...)

#include <vector>

namespace reflect {
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
}

#endif //REFLECTOR_REFLECT_HPP
