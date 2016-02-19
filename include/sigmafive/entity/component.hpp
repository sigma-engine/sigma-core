#ifndef SIGMAFIVE_ENTITY_COMPONENT_HPP
#define SIGMAFIVE_ENTITY_COMPONENT_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/entity/component_cache.hpp>

#include <bitset>

#define SIGMA_FIVE_ENTITY_COMPONENT(CLASS) namespace sigmafive { namespace entity { \
    template<> struct component_type_traits<CLASS> {\
        static constexpr const component_type type = cppbr::meta::compile_time_hash(#CLASS);\
        static constexpr const char *name = #CLASS;\
    };\
}}

#define SIGMA_FIVE_ENTITY_COMPONENT_ID(CLASS,ID) namespace sigmafive { namespace entity { \
    template<> struct component_type_traits<CLASS> {\
        static constexpr const component_type type = ID;\
        static constexpr const char *name = #CLASS;\
    };\
}}

namespace sigmafive {
    namespace entity {
        typedef std::uint64_t component_type;
        typedef std::bitset<64> component_mask_type; //TODO use a dynamic bitset to avoid this max!

        template<class T>
        struct component_type_traits {};

        template<class T>
        using component = typename component_cache<T>::handle_type;
    }
}

#endif //SIGMAFIVE_ENTITY_COMPONENT_HPP
