#include <game/entity_world.hpp>

namespace sigmafive {
    namespace game {
        component_bitset_manager &entity_world::bitset_manager() {
            return component_manager_.bitset_manager();
        }

        game::entity entity_world::create() {
			return entity_manager_.create();
		}

		bool entity_world::is_alive(game::entity e) const {
			return entity_manager_.is_alive(e);
		}

		void entity_world::destroy(game::entity e) {
            auto premask = component_manager_.get_component_mask(e);
            component_manager_.remove_all_components(e);
            auto postmask = component_manager_.get_component_mask(e);
            for(auto &item:component_system_manager_) {
                component_system *system = item.second.get();
                if(system->is_intrested(premask) && !system->is_intrested(postmask))
                    system->entity_removed(e);
            }
			entity_manager_.destroy(e);
		}
	}
}
