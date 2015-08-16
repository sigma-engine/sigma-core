#ifndef SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP
#define SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP

#include <sigmafive/game/entity_world.hpp>
#include <sigmafive/game/component_system.hpp>
#include <sigmafive/game/component_manager.hpp>
#include <sigmafive/game/transform_component.hpp>
#include <sigmafive/game/static_mesh_component.hpp>
#include <sigmafive/game/scene.hpp>

namespace sigmafive {
    namespace game {
        class static_mesh_component_system : public component_system {
            SIGMAFIVE_CLASS()
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

