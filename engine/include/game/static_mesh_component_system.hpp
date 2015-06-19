#ifndef SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP
#define SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP

#include <game/entity_world.hpp>
#include <game/component_system.hpp>
#include <game/component_manager.hpp>
#include <game/transform_component.hpp>
#include <game/static_mesh_component.hpp>
#include <game/scene.hpp>

namespace sigmafive {
	namespace game {
		class static_mesh_component_system : public component_system {
            SIGMAFIVE_OBJECT()
		public:
            void init(game::entity_world &world,game::scene &scene);

			virtual ~static_mesh_component_system();

			virtual void entity_added(entity e) override;

			virtual void entity_removed(entity e) override;

			virtual void process() override;
		private:
            entity_world *world_;
            scene *scene_;
		};
	}
}

#endif //SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP
