#ifndef SIGMA_RESOURCE_HPP
#define SIGMA_RESOURCE_HPP

#include <sigma/handle.hpp>


#define REGISTER_RESOURCE(Klass, ShortName, Version)             \
    namespace sigma {                                            \
    namespace resource {                                         \
        template <>                                              \
        struct resource_traits<Klass> {                          \
            static constexpr const char* fullname = #Klass;      \
            static constexpr const char* shortname = #ShortName; \
        };                                                       \
    }                                                            \
    }

#define resource_name(Klass) sigma::resource::resource_traits<Klass>::fullname
#define resource_shortname(Klass) sigma::resource::resource_traits<Klass>::shortname

namespace sigma {
namespace resource {
    template <class Resource>
    using handle = sigma::handle<Resource>;

    template <class T>
    struct resource_traits;
}
}

#endif // SIGMA_RESOURCE_HPP
