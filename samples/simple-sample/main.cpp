#include <sigmafive/engine.hpp>

#include <sigmafive/game/entity_manager.hpp>
#include <sigmafive/game/component_manager.hpp>
#include <sigmafive/game/component_system_manager.hpp>
#include <sigmafive/game/static_mesh_component_system.hpp>

int main(int argc, char *argv[]) {
    sigmafive::engine engine(argc, argv);

    sigmafive::game::entity_manager entity_manager;
    sigmafive::game::component_manager component_manager(engine.component_registry());
    sigmafive::game::component_system_manager component_system_manager;

    auto window = engine.window_manager().create_window(
            sigmafive::compile_time_hash("sigmafive::system::SDL2::window"));

    auto system = component_system_manager.add_component_system<sigmafive::game::static_mesh_component_system>();
    system->init(&engine.graphics_context_manager());

    while (window->good()) {
    }

    return 0;
}
