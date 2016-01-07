#ifndef SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP
#define SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>
#include <cppbr/meta/factory.hpp>

#include <sigmafive/entity/component.hpp>

namespace sigmafive {
    namespace graphics {
        class static_mesh;
    }
    namespace game {
        class SIGMAFIVE_API static_mesh_component : public cppbr::meta::object {
        CPPBR_META_CLASS()
        public:
            void set_static_mesh(std::shared_ptr<graphics::static_mesh> static_mesh);

            std::weak_ptr<graphics::static_mesh> static_mesh();
        private:
            std::shared_ptr<graphics::static_mesh> static_mesh_;
        };

        using static_mesh_component_pool = sigmafive::entity::component_pool<static_mesh_component>;
        using static_mesh_component_pool_factory = cppbr::meta::simple_factory<sigmafive::entity::base_component_pool, static_mesh_component_pool>;
    }
}

#endif //SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP
