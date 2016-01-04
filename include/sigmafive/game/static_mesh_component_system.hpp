#ifndef SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP
#define SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>

#include <sigmafive/entity/component_system.hpp>

namespace sigmafive {
    namespace graphics {
        class context_manager;
    }
    namespace game {
        class SIGMAFIVE_API static_mesh_component_system : public entity::component_system {
        CPPBR_META_CLASS()
        public:
            void init(graphics::context_manager *context_manager);

            virtual ~static_mesh_component_system();

            virtual void process(entity::entity_manager *em, entity::component_manager *cm) override;

        private:
            graphics::context_manager *context_manager_;
        };
    }
}

#endif //SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP

