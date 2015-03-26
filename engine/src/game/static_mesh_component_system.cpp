#include <game/static_mesh_component_system.hpp>
#include <iostream>
namespace sigmafive {
	namespace game {
		static_mesh_component_system::static_mesh_component_system(game::scene &scene)
				: scene_(scene) {
		}

		static_mesh_component_system::~static_mesh_component_system() {
		}

		bool static_mesh_component_system::is_intrested(entity e) const {
			auto b = detail::bitset_builder<transform_component,static_mesh_component>{}();
			return (e.component_mask() & b) == b;
		}

		void static_mesh_component_system::entity_added(entity e) {
			std::cout << "entity added." << std::endl;
		}

		void static_mesh_component_system::entity_removed(entity e) {
			std::cout << "entity removed." << std::endl;
		}

		void static_mesh_component_system::process(entity_system &entity_system) {

		}
	}
}

SIGMAFIVE_IMPLEMENT_COMPONENT_SYSTEM(sigmafive::game::static_mesh_component_system);