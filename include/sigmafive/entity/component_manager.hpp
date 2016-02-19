#ifndef SIGMAFIVE_ENTITY_COMPONENT_MANAGER_HPP
#define SIGMAFIVE_ENTITY_COMPONENT_MANAGER_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/entity/entity.hpp>
#include <sigmafive/entity/component.hpp>
#include <sigmafive/entity/component_cache.hpp>
#include <sigmafive/entity/component_cache_factory.hpp>

#include <memory>
#include <bitset>
#include <vector>
#include <unordered_map>

namespace sigmafive {
    namespace entity {

        class SIGMAFIVE_API component_manager {
        public:
            typedef component_cache_factory::cache_type_id component_type;
            typedef component_cache_factory::cache_type_mask_type component_mask_type;

            component_manager(component_cache_factory &reg) : reg(reg) {}

            component_manager(const component_manager &) = delete;

            component_manager &operator=(const component_manager &) = delete;

            void add_component(component_type type, entity e);

            bool has_component(component_type type, entity e) const noexcept;

            void remove_component(component_type type, entity e);

            void clear();

            template<class T>
            component<T> add_component(entity e) {
                add_component(component_type_traits<T>::type,e);
                return get_component<T>(e);
            }

            template<class T>
            bool has_component(entity e) const noexcept {
                return has_component(component_type_traits<T>::type,e);
            }

            template<class T>
            void remove_component(entity e) {
                remove_component(component_type_traits<T>::type,e);
            }

            template<class T>
            component<T> get_component(entity e) {
                if(has_component<T>(e)) {
                    auto type = component_type_traits<T>::type;
                    return static_cast<component_cache<T>*>(component_caches[type].get())->get(e);
                }
                return nullptr;
            }

        private:
            component_cache_factory &reg;
            std::vector<component_mask_type> component_masks;
            std::unordered_map<component_type, std::unique_ptr<base_component_cache>> component_caches;
        };
    }
}

#endif //SIGMAFIVE_ENTITY_COMPONENT_MANAGER_HPP
