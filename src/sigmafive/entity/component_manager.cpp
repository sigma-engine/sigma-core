#include <sigmafive/entity/component_manager.hpp>
#include <sigmafive/entity/component_cache.hpp>

namespace sigmafive {
    namespace entity {

        void component_manager::add_component(component_manager::component_type type, entity e) {
            if(e.is_valid() && !has_component(type,e)) {
                if(e.index >= component_masks.size())
                    component_masks.resize(e.index + 1);
                if(component_caches.count(type) == 0) {
                    component_caches[type] = reg.create_cache(type);
                }
                component_caches[type]->add(e);
                component_masks[e.index] |= reg.mask_for(type);
            }
        }

        bool component_manager::has_component(component_manager::component_type type, entity e) const noexcept {
            if(e.is_valid() && e.index < component_masks.size()) {
                auto mask = reg.mask_for(type);
                return (component_masks[e.index] & mask) == mask;
            }
            return false;
        }

        void component_manager::remove_component(component_manager::component_type type, entity e) {
            if(e.is_valid() && e.index < component_masks.size() && has_component(type,e)) {
                component_masks[e.index] &= ~reg.mask_for(type);
                component_caches[type]->remove(e);
            }
        }

        void component_manager::clear() {
            reg.clear();
            component_masks.clear();
            component_caches.clear();
        }

    }
}
