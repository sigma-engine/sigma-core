#include <sigmafive/engine.hpp>
#include <sigmafive/system/window.hpp>
#include <sigmafive/game/entity_manager.hpp>
#include <sigmafive/game/component_manager.hpp>
#include <sigmafive/game/component_system_manager.hpp>
#include <sigmafive/game/static_mesh_component_system.hpp>

int main(int argc, char *argv[]) {
    sigmafive::engine engine(argc,argv);
   
    sigmafive::game::entity_manager entity_manager;
    sigmafive::game::component_manager component_manager(engine.component_registry());
    sigmafive::game::component_system_manager component_system_manager;

    auto system = component_system_manager.add_component_system<sigmafive::game::static_mesh_component_system>();
    system->init(&engine.graphics_context_manager());

    return 0;
}
