#include <sigmafive/game/static_mesh_component_system.hpp>

#include <sigmafive/entity/entity_manager.hpp>
#include <sigmafive/entity/component_manager.hpp>
#include <sigmafive/game/transform_component.hpp>
#include <sigmafive/game/static_mesh_component.hpp>

#include <sigmafive/graphics/context_manager.hpp>


#include <iostream>

namespace sigmafive {
    namespace game {

        void static_mesh_component_system::init(graphics::context_manager *context_manager) {
            context_manager_ = context_manager;
        }

        static_mesh_component_system::~static_mesh_component_system() {
        }

        void static_mesh_component_system::process(entity::entity_manager *em, entity::component_manager *cm) {
            sigmafive::graphics::context *context = context_manager_->current_context();
            if (!context)
                return;

            //TODO this is a mess
            for (entity::entity e:*em) {
                if (cm->has_component<transform_component>(e) && cm->has_component<static_mesh_component>(e)) {
                    auto tcmp = cm->get_component<transform_component>(e);
                    auto scmp = cm->get_component<static_mesh_component>(e);
                    context->add_static_mesh(tcmp->matrix(),scmp->static_mesh());
                }
            }
        }
    }
}