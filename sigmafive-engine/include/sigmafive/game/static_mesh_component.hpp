#ifndef SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP
#define SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP

#include <sigmafive/game/component.hpp>
#include <sigmafive/graphics/static_mesh.hpp>
#include <sigmafive/game/component_registry.hpp>

namespace sigmafive {
    namespace game {
        class static_mesh_component : public game::component {
            SIGMAFIVE_CLASS()
        public:
            boost::uuids::uuid static_mesh;
        };

        using static_mesh_component_pool = simple_component_pool<static_mesh_component>;
        using static_mesh_component_pool_factory = simple_factory<component_pool,static_mesh_component_pool>;
    }
}

#endif //SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP
