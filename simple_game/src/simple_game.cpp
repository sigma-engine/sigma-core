#include "simple_game.hpp"

#include <sigma/world.hpp>

#include <boost/math/constants/constants.hpp>

#include <chrono>
#include <cmath>

simple_game::simple_game(simple_context& ctx)
{
    auto blueprint = ctx.get_cache<simple_blueprint>().acquire({ 0, 0 });

    for (const auto& entity : blueprint->entities) {
        auto e = world_.create();
        for (const auto& component : entity) {
            simple_component_set::for_each([&](auto type_tag) {
                using component_type = typename decltype(type_tag)::type;
                if (const component_type* cmp = boost::get<component_type>(&component))
                    world_.add<component_type>(e, *cmp);
            });
        }
    }

    world_.for_each<sigma::transform, sigma::graphics::static_mesh_instance, grid_component>([&](sigma::entity e, const sigma::transform& txform, sigma::graphics::static_mesh_instance& mesh_instance, const grid_component& grid) {
        for (int x = 0; x < grid.rows; ++x) {
            for (int z = 0; z < grid.columns; ++z) {
                if (x != 0 || z != 0) {
                    auto e = world_.create();
                    world_.add<sigma::transform>(e, txform.position + glm::vec3{ grid.row_spacing * x, 0, grid.column_spacing * z });
                    world_.add<sigma::graphics::static_mesh_instance>(e, mesh_instance.mesh);
                }
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

    world_.for_each<sigma::transform>([&](sigma::entity e, sigma::transform& txform) {
        txform.matrix = txform.get_matrix();
    });

    world_.for_each<sigma::transform, sigma::graphics::directional_light>([&](sigma::entity e, const sigma::transform& txform, sigma::graphics::directional_light& light) {
        light.direction = glm::normalize(glm::vec3(txform.matrix * glm::vec4(0, 1, 0, 0)));
        // light.shadow_frustum.set_view(glm::lookAt(txform.position, txform.position - light.direction, glm::vec3(0, 1, 0)));
    });

    world_.for_each<sigma::transform, sigma::graphics::spot_light>([&](sigma::entity e, const sigma::transform& txform, sigma::graphics::spot_light& light) {
        light.direction = glm::normalize(glm::vec3(txform.matrix * glm::vec4(0, 1, 0, 0)));
        light.shadow_frustum.set_projection_view(2.5f * light.cutoff, 1.0f, 0.01f, 50.0f, glm::lookAt(txform.position, txform.position - light.direction, glm::vec3{ 0.0f, 1.0f, 0.0f }));
    });
}
