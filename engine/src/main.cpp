#include <math/vec3.hpp>
#include <system/window.hpp>

#include <game/scene.hpp>
#include <graphics/static_mesh.hpp>
#include <system/resource_manager.hpp>
#include <graphics/opengl/scene_renderer.hpp>
#include <game/entity_manager.hpp>

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

    while(window.good()) {
        scene_renderer.render(float4x4(),scene);
    }

    return 0;
}
