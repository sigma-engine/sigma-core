#include <sigmafive/game/component_manager.hpp>

namespace sigmafive {
    namespace game {
        component_bitset_manager &component_manager::bitset_manager() {
            return bitset_manager_;
        }

        bitset component_manager::get_component_mask(entity e) const {
            if(e.index >= component_masks_.size())
                return bitset{};
            return component_masks_[e.index];
        }

        component *component_manager::add_component(class_hash component_id, entity e) {
            auto it = component_pools_.find(component_id);
            if(it == component_pools_.end())
                it = component_pools_.emplace(std::make_pair(component_id,object::meta_class_for(component_id).create_pool())).first;

            if(e.index >= component_masks_.size())
                component_masks_.resize(e.index+1);

            component_masks_[e.index] |= bitset_manager_.bitset_for(component_id);

            return dynamic_cast<component_pool_base*>(it->second.get())->add_component(e);
        }

        bool component_manager::has_component(class_hash component_id, entity e) {
            if(e.index >= component_masks_.size())
                return false;
            auto bs = bitset_manager_.bitset_for(component_id);
            return (component_masks_[e.index] & bs) == bs;
        }

        component *component_manager::get_component(class_hash component_id, entity e) {
            auto it = component_pools_.find(component_id);
            if(it == component_pools_.end())
                return nullptr;

            return dynamic_cast<component_pool_base*>(it->second.get())->get_component(e);
        }

        void component_manager::remove_component(class_hash component_id,entity e) {
            auto it = component_pools_.find(component_id);
            if(it == component_pools_.end() || e.index >= component_masks_.size())
                return;
            dynamic_cast<component_pool_base*>(it->second.get())->remove_component(e);
            component_masks_[e.index] &= bitset_manager_.bitset_for(component_id).flip();
        }

        void component_manager::remove_all_components(entity e) {
            if(e.index >= component_masks_.size())
                return;
            for(auto &item:component_pools_) {
                component_pool_base *pool = static_cast<component_pool_base*>(item.second.get());
                if(pool)
                    pool->remove_component(e);
            }
            component_masks_[e.index] = bitset(0);
        }
    }
}
