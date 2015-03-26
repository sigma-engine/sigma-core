#include <game/static_mesh_component_system.hpp>

namespace sigmafive {
	namespace game {
		static_mesh_component_system::static_mesh_component_system() {

		}

		static_mesh_component_system::~static_mesh_component_system() {
		}

		bool static_mesh_component_system::is_intrested(entity e) const {
			auto required = detail::bitset_builder<transform_component,static_mesh_component>{}();
			return (e.component_mask() & required) == required;
		}

		void static_mesh_component_system::entity_added(entity e) {

		}

		void static_mesh_component_system::entity_removed(entity e) {

		}

		void static_mesh_component_system::process(entity_system &entity_system) {

		}
	}
}

SIGMAFIVE_IMPLEMENT_COMPONENT_SYSTEM(sigmafive::game::static_mesh_component_system);