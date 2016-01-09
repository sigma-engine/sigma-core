#ifndef SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP
#define SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/entity/component.hpp>

#include <cppbr/meta/object.hpp>

#include <memory>

namespace sigmafive {
    namespace graphics {
        class static_mesh;
    }
    namespace game {
        class SIGMAFIVE_API static_mesh_component {
        public:
            void set_static_mesh(std::shared_ptr<graphics::static_mesh> static_mesh);

            std::weak_ptr<graphics::static_mesh> static_mesh();
        private:
            std::shared_ptr<graphics::static_mesh> static_mesh_;
        };
    }
}

SIGMA_FIVE_ENTITY_COMPONENT(sigmafive::game::static_mesh_component)

#endif //SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP
