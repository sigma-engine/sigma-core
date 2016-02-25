#ifndef SIGMAFIVE_ENTITY_COMPONENT_CACHE_FACTORY_HPP
#define SIGMAFIVE_ENTITY_COMPONENT_CACHE_FACTORY_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/entity/component.hpp>

#include <cppbr/cache/cache_factory.hpp>

namespace sigmafive {
    namespace entity {
        class base_component_cache;

        class component_cache_factory : public cppbr::cache::cache_factory<sigmafive::entity::component_type,sigmafive::entity::component_mask_type,base_component_cache> {
            typedef cppbr::cache::cache_factory<sigmafive::entity::component_type,sigmafive::entity::component_mask_type,base_component_cache> base;
        public:
            component_cache_factory() = default;

            // TODO move these to cppbr::cache::cache_factory?
            component_cache_factory(const component_cache_factory &) = delete;

            component_cache_factory &operator=(const component_cache_factory &) = delete;

            template<class T>
            void register_cache(cache_factory_type pool_factory) {
                base::register_cache(component_type_traits<T>::type,pool_factory);
            }

            template<class T>
            void unregister_cache() {
                base::unregister_cache(component_type_traits<T>::type);
            }
        };
    }
}

#endif //SIGMAFIVE_ENTITY_COMPONENT_CACHE_FACTORY_HPP
