#include <sigmafive/engine.hpp>
#include <sigmafive/system/window.hpp>
#include <sigmafive/game/entity_manager.hpp>
#include <sigmafive/game/component_manager.hpp>
#include <sigmafive/game/component_system_manager.hpp>
#include <sigmafive/game/static_mesh_component_system.hpp>

int main(int argc, char *argv[]) {
    sigmafive::engine engine(argc,argv);
    sigmafive::system::context_attributes context_attributes;
    context_attributes.red = 8;
    context_attributes.green = 8;
    context_attributes.blue = 8;
    context_attributes.alpha = 8;
    context_attributes.depth = 24;
    context_attributes.stencil = 8;
    context_attributes.samples = 0;
    context_attributes.major = 4;
    context_attributes.minor = 4;
    context_attributes.double_buffer = true;
    context_attributes.core_profile = false;
    context_attributes.vsync = false;
    sigmafive::system::window window("sigma five sample", int2(800, 600), context_attributes);

    sigmafive::game::entity_manager entity_manager;
    sigmafive::game::component_manager component_manager(engine.component_registry());
    sigmafive::game::component_system_manager component_system_manager;

    auto system = component_system_manager.add_component_system<sigmafive::game::static_mesh_component_system>();
    system->init(&engine.graphics_context_manager());

    while(window.good()) {
    }

    return 0;
}
