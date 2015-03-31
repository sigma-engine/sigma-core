#include <game/static_mesh_component_system.hpp>
#include <game/entity_manager.hpp>

namespace sigmafive {
	namespace game {
		static_mesh_component_system::static_mesh_component_system(game::entity_manager &entity_manager,game::scene &scene)
				: component_system(entity_manager), scene_(scene) {
		}

		static_mesh_component_system::~static_mesh_component_system() {
		}

		bool static_mesh_component_system::is_intrested(entity e) const {
			auto b = detail::bitset_builder<transform_component,static_mesh_component>{}();
			return (e.component_mask() & b) == b;
		}

		void static_mesh_component_system::entity_added(entity e) {
			auto trans = entity_manager_.get_component<transform_component>(e);
			auto mesh = entity_manager_.get_component<static_mesh_component>(e);
			scene_.add_static_mesh(e.id().index,trans,mesh);
		}

		void static_mesh_component_system::entity_removed(entity e) {
			scene_.remove_static_mesh(e.id().index);
		}

		void static_mesh_component_system::process() {

		}
	}
}

SIGMAFIVE_IMPLEMENT_COMPONENT_SYSTEM(sigmafive::game::static_mesh_component_system);