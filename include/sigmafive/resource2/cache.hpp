#pragma once

#include <sigmafive/config.hpp>

#include <sigmafive/resource2/identifier.hpp>

#include <cppbr/cache/cache.hpp>
#include <cppbr/cache/handle.hpp>

namespace sigmafive {
    namespace resource2 {
        class SIGMAFIVE_API base_resource_cache {
        public:
            virtual ~base_resource_cache() {}
        };

        template<class T>
        using resource_cache = cppbr::cache::cache<identifier,T,base_resource_cache>;

        template<class T>
        using resource = typename resource_cache<T>::handle_type;
    }
}
