#include <sigmafive/game/static_mesh_component_system.hpp>
#include <sigmafive/game/entity_manager.hpp>

namespace sigmafive {
    namespace game {

        void static_mesh_component_system::init(entity_world &world,game::scene &scene) {
            world_ = &world;
            scene_ = &scene;
            intrested_mask_ |= world_->bitset_manager().bitset_for<static_mesh_component>();
            intrested_mask_ |= world_->bitset_manager().bitset_for<transform_component>();
        }

        static_mesh_component_system::~static_mesh_component_system() {
        }

        void static_mesh_component_system::entity_added(entity e) {
            scene_->add_static_mesh(e.index,world_->get_component<transform_component>(e),world_->get_component<static_mesh_component>(e));
        }

        void static_mesh_component_system::entity_removed(entity e) {
            scene_->remove_static_mesh(e.index);
        }

        void static_mesh_component_system::process() {

        }
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::game::static_mesh_component_system)
