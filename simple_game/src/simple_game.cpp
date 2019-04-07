#include "simple_game.hpp"

#include <chrono>
#include <cmath>

simple_game::simple_game(simple_context& ctx)
{
    auto bp = ctx.get_cache<simple_blueprint>().acquire(ctx.get_settings<simple_level_settings>().current_level_blueprint);
    registry = std::move(bp->registry);

    registry.view<sigma::transform, grid_component>().each([&](auto e, const sigma::transform& txform, const grid_component& grid) {
        for (int x = 0; x < grid.rows; ++x) {
            for (int z = 0; z < grid.columns; ++z) {
                auto e = registry.create();
                registry.assign<sigma::transform>(e, txform.position + glm::vec3{ grid.row_spacing * x, 0, grid.column_spacing * z }, txform.rotation, txform.scale);
                registry.assign<sigma::graphics::static_mesh_instance>(e, grid.mesh);
            }
        }
    });
}

void simple_game::update(std::chrono::duration<float> dt)
{
    // static auto start = std::chrono::system_clock::now();
    // auto elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::system_clock::now() - start);
    // world_.for_each<sigma::transform, sigma::graphics::point_light>([elapsed, dt](sigma::entity e, sigma::transform& txform, const sigma::graphics::point_light& light) {
    //     txform.position.y += std::cos(elapsed.count()) * dt.count();
    // });

    registry.view<sigma::transform>().each([&](auto& txform) {
        txform.matrix = txform.get_matrix();
    });

    registry.view<sigma::transform, sigma::graphics::directional_light>().each([&](auto& txform, auto& light) {
        light.direction = glm::normalize(glm::vec3(txform.matrix * glm::vec4(0, 1, 0, 0)));
        // light.shadow_frustum.set_view(glm::lookAt(txform.position, txform.position - light.direction, glm::vec3(0, 1, 0)));
    });

    registry.view<sigma::transform, sigma::graphics::spot_light>().each([&](auto& txform, auto& light) {
        light.direction = glm::normalize(glm::vec3(txform.matrix * glm::vec4(0, 1, 0, 0)));
        light.shadow_frustum.set_projection_view(2.5f * light.cutoff, 1.0f, 0.01f, 50.0f, glm::lookAt(txform.position, txform.position - light.direction, glm::vec3{ 0.0f, 1.0f, 0.0f }));
    });
}
