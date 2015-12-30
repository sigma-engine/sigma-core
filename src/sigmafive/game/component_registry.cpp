#include <sigmafive/game/component_registry.hpp>

namespace sigmafive {
    namespace game {
        void component_registry::register_component(class_uid uid,
                                                    std::unique_ptr<factory<component_pool>> pool_factory) {
            registered_components_[uid] = {uid, std::move(pool_factory), 0};
            registered_components_[uid].mask.set(registered_components_.size() - 1);
        }

        component_mask component_registry::mask_for(class_uid uid) const {
            return registered_components_.at(uid).mask;
        }

        std::unique_ptr<component_pool> component_registry::create_component_pool_for(class_uid uid) const {
            return registered_components_.at(uid).pool_factory->create();
        }
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::game::component_pool)
EXPORT_SIGMAFIVE_CLASS(sigmafive::game::component_registry)