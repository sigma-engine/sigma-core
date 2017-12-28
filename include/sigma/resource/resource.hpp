#ifndef SIGMA_RESOURCE_HPP
#define SIGMA_RESOURCE_HPP

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>

#define REGISTER_RESOURCE(Klass, ShortName, Version)         \
    namespace sigma {                                        \
    template <>                                              \
    struct resource_traits<Klass> {                          \
        static constexpr const char* fullname = #Klass;      \
        static constexpr const char* shortname = #ShortName; \
    };                                                       \
    }                                                        \
    BOOST_CLASS_VERSION(Klass, Version)

#define resource_name(Klass) sigma::resource_traits<Klass>::fullname
#define resource_shortname(Klass) sigma::resource_traits<Klass>::shortname

namespace sigma {
template <class T>
struct resource_traits {
};
}

#endif // SIGMA_RESOURCE_HPP
