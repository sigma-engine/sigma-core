#ifndef SIGMAFIVE_GAME_SCENE_HPP
#define SIGMAFIVE_GAME_SCENE_HPP

#include <sigmafive/game/entity.hpp>
#include <sigmafive/game/transform_component.hpp>
#include <sigmafive/game/static_mesh_component.hpp>

#include <queue>
#include <unordered_map>

namespace sigmafive {
    namespace game {

        struct static_mesh_instance {
            transform_component *transform;
            static_mesh_component *static_mesh;
        };

        class scene {
        public:
            void add_static_mesh(std::uint32_t id,transform_component *transform,static_mesh_component *static_mesh);

            std::queue<static_mesh_instance> static_meshes() const;

            void remove_static_mesh(std::uint32_t id);
        private:
            std::unordered_map<std::uint32_t,static_mesh_instance> static_meshes_;
        };
    }
}

#endif //SIGMAFIVE_GAME_SCENE_HPP
