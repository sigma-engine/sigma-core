#include <sigmafive/game/component_manager.hpp>

#include <sigmafive/game/component_registry.hpp>

namespace sigmafive {
    namespace game {

        component_manager::component_manager(component_registry *registry)
                : registry_(registry) {
        }

        component_mask component_manager::get_component_mask(entity e) const {
            if (e.index >= component_masks_.size())
                return component_mask{};
            return component_masks_[e.index];
        }

        component *component_manager::add_component(class_uid component_id, entity e) {
            auto it = component_pools_.find(component_id);
            if (it == component_pools_.end())
                it = component_pools_.emplace(
                        std::make_pair(component_id, registry_->create_component_pool_for(component_id))).first;

            if (e.index >= component_masks_.size())
                component_masks_.resize(e.index + 1);

            component_masks_[e.index] |= registry_->mask_for(component_id);

            return it->second.get()->add_component(e);
        }

        bool component_manager::has_component(class_uid component_id, entity e) {
            if (e.index >= component_masks_.size())
                return false;
            auto bs = registry_->mask_for(component_id);
            return (component_masks_[e.index] & bs) == bs;
        }

        component *component_manager::get_component(class_uid component_id, entity e) {
            auto it = component_pools_.find(component_id);
            if (it == component_pools_.end())
                return nullptr;

            return it->second.get()->get_component(e);
        }

        void component_manager::remove_component(class_uid component_id, entity e) {
            auto it = component_pools_.find(component_id);
            if (it == component_pools_.end() || e.index >= component_masks_.size())
                return;
            it->second.get()->remove_component(e);
            component_masks_[e.index] &= registry_->mask_for(component_id).flip();
        }

        void component_manager::remove_all_components(entity e) {
            if (e.index >= component_masks_.size())
                return;
            for (auto &item:component_pools_) {
                if (item.second)
                    item.second->remove_component(e);
            }
            component_masks_[e.index] = component_mask(0);
        }
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::game::component_manager)