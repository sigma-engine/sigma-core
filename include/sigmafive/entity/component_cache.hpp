#ifndef SIGMA_FIVE_ENTITY_COMPONENT_CACHE_HPP
#define SIGMA_FIVE_ENTITY_COMPONENT_CACHE_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/entity/entity.hpp>

#include <cppbr/cache/cache.hpp>
#include <cppbr/cache/handle.hpp>

namespace sigmafive {
    namespace entity {
        class SIGMAFIVE_API base_component_cache {
        public:
            base_component_cache() = default;

            base_component_cache(const base_component_cache &) = delete;

            base_component_cache &operator=(const base_component_cache &) = delete;

            virtual ~base_component_cache() {}

            virtual void add(entity e) = 0;

            virtual bool remove(entity e) = 0;
        };

        template<class T>
        using component_cache = cppbr::cache::cache<entity,T,base_component_cache>;
    }
}

#endif //SIGMA_FIVE_ENTITY_COMPONENT_CACHE_HPP
