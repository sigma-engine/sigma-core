#ifndef SIMPLE_GAME_HPP
#define SIMPLE_GAME_HPP

#include <grid_component.hpp>

#include <sigma/context.hpp>
#include <sigma/game.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/transform.hpp>
#include <sigma/world.hpp>

#include <glm/vec3.hpp>

using simple_context = sigma::context<sigma::graphics::texture,
    sigma::graphics::cubemap,
    sigma::graphics::shader,
    sigma::graphics::technique,
    sigma::graphics::material,
    sigma::graphics::static_mesh,
    sigma::graphics::post_process_effect>;

using simple_world = sigma::world<sigma::transform,
    sigma::graphics::directional_light,
    sigma::graphics::point_light,
    sigma::graphics::spot_light,
    sigma::graphics::static_mesh_instance,
    grid_component>;

class simple_game : public sigma::game<simple_world> {
public:
    simple_game(simple_context& ctx);

    virtual void update(std::chrono::duration<float> dt) override;
};

#endif // SIMPLE_GAME_HPP
