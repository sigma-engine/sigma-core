#ifndef SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP
#define SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/object.hpp>

#include <sigmafive/game/component_system.hpp>
#include <sigmafive/game/component_manager.hpp>
#include <sigmafive/game/transform_component.hpp>
#include <sigmafive/game/static_mesh_component.hpp>

#include <sigmafive/graphics/context_manager.hpp>

namespace sigmafive {
    namespace game {
        class SIGMAFIVE_API static_mesh_component_system : public component_system {
			SIGMAFIVE_CLASS()
        public:
            void init(graphics::context_manager *context_manager);

            virtual ~static_mesh_component_system();

            virtual void process(entity_manager &em,component_manager &cm) override;
        private:
            graphics::context_manager *context_manager_;
        };
    }
}

#endif //SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP

