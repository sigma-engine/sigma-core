#include <sigmafive/engine.hpp>
#include <sigmafive/graphics/opengl/context.hpp>
#include <sigmafive/game/transform_component.hpp>
#include <sigmafive/game/static_mesh_component.hpp>

namespace sigmafive {

    engine::engine(int &argc, char **argv) {
        graphics_context_manager_.register_context(graphics::opengl::context::CLASS_ID,
                                                   std::unique_ptr<graphics::context_factory>{new graphics::opengl::context_factory{resource_manager_}});

        component_registry_.register_component(game::transform_component::CLASS_ID,
                                                       std::unique_ptr<game::transform_component_pool_factory>(new game::transform_component_pool_factory{}));
        component_registry_.register_component(game::static_mesh_component::CLASS_ID,
                                                       std::unique_ptr<game::static_mesh_component_pool_factory>(new game::static_mesh_component_pool_factory{}));
    }

    system::resource_manager &engine::resource_manager() {
        return resource_manager_;
    }

    game::component_registry &engine::component_registry() {
        return component_registry_;
    }

    graphics::context_manager &engine::graphics_context_manager() {
        return graphics_context_manager_;
    }
}
