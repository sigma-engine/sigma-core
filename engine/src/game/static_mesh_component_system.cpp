#include <game/static_mesh_component_system.hpp>
#include <game/entity_manager.hpp>

namespace sigmafive {
	namespace game {
		static_mesh_component_system::~static_mesh_component_system() {
		}

		bool static_mesh_component_system::is_intrested(bitset mask) const {
			return false;
		}

		void static_mesh_component_system::entity_added(entity e) {

		}

		void static_mesh_component_system::entity_removed(entity e) {

		}

		void static_mesh_component_system::process() {

		}
	}
}

SIGMAFIVE_IMPLEMENT_COMPONENT_SYSTEM(sigmafive::game::static_mesh_component_system);