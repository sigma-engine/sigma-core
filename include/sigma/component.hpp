#ifndef SIGMA_COMPONENT_HPP
#define SIGMA_COMPONENT_HPP

#define REGISTER_COMPONENT(X)                         \
    namespace sigma {                               \
    template <>                                     \
    struct component_traits<X> {                    \
        static constexpr const char* fullname = #X; \
    };                                              \
    }

#define component_name(X) sigma::component_traits<X>::fullname

namespace sigma {
template <class T>
struct component_traits {
};
}

#endif // SIGMA_COMPONENT_HPP
