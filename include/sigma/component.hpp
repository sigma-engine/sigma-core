#ifndef SIGMA_COMPONENT_HPP
#define SIGMA_COMPONENT_HPP

#include <sigma/util/type_sequence.hpp>

#define REGISTER_COMPONENT(X)                       \
    namespace sigma {                               \
    template <>                                     \
    struct component_traits<X> {                    \
        static constexpr const char* fullname = #X; \
    };                                              \
    }

#define component_name(X) sigma::component_traits<X>::fullname

namespace sigma {
template <class... Components>
using component_set = type_set_t<Components...>;

using component_mask_type = std::uint64_t;

template <class T>
struct component_traits {
};
}

#endif // SIGMA_COMPONENT_HPP
