#include <math/vec3.hpp>
#include <game/scene.hpp>
#include <system/window.hpp>
#include <game/entity_manager.hpp>
#include <system/resource_manager.hpp>
#include <graphics/opengl/scene_renderer.hpp>
#include <game/static_mesh_component_system.hpp>

class test : public sigmafive::game::component {
SIGMAFIVE_COMPONENT();

};
SIGMAFIVE_IMPLEMENT_COMPONENT(test);

int main(int argc, char const *argv[]) {
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

    sigmafive::system::resource_manager resource_manager;
    sigmafive::graphics::opengl::scene_renderer scene_renderer(resource_manager);

    sigmafive::game::scene scene;
    sigmafive::game::entity_manager entity_manager;
    entity_manager.add_system<sigmafive::game::static_mesh_component_system>();

    auto e = entity_manager.create();
    entity_manager.add_component<sigmafive::game::transform_component>(e);
    entity_manager.add_component<sigmafive::game::static_mesh_component>(e);
    entity_manager.add_component<test>(e);
    entity_manager.remove_component<test>(e);

    while(window.good()) {
        scene_renderer.render(float4x4(),scene);
    }

    return 0;
}
