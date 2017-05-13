#include "simple_game.hpp"

#include <glm/vec3.hpp>

#include <boost/math/constants/constants.hpp>

#include <chrono>
#include <cmath>

simple_game::simple_game(sigma::graphics::renderer* renderer)
    : sigma::game<simple_world>(renderer)
    , generator_(6546534)
    , position_distribution_{ { -50, .5f, -50 }, { 50.0f, .5f, 50.0f } }
    , rotation_distribution_{ glm::vec3{ 0.0f }, glm::vec3{ 2.0f * boost::math::constants::pi<float>() } }
    , scale_distribution_{ 0.1f, 10.0f }
    , color_distribution_{ glm::vec3{ 0.0f }, glm::vec3{ 1.0f } }
{

    // load("../data/proprietary/classroom/classroom.scn");
    // load("../data/water_packed.scn");
    // load("../data/material_test_scene.scn");
    load("../data/proprietary/sponza/sponza.scn");

    // auto grid_e = world_.create();
    // world_.add<sigma::transform>(grid_e);
    // world_.add<sigma::graphics::static_mesh_instance>(grid_e, renderer->static_meshes().get(sigma::resource::identifier{ "static_mesh://material_ball:material_ball" }));
    // world_.add<grid_component>(grid_e, 5, 5, 1.5f, 1.5f);

    world_.for_each<sigma::transform, sigma::graphics::static_mesh_instance, grid_component>([&](sigma::entity e, const sigma::transform& txform, sigma::graphics::static_mesh_instance& mesh_instance, const grid_component& grid) {
        auto material = mesh_instance.mesh->material(0);
        material->set_uniform("roughness", 0.0f);
        material->set_uniform("basecolor", glm::vec3{ 1.0f, 0.0f, 0.0f });
        material->set_uniform("metalness", 1.0f);

        int number = 0;
        for (int x = 0; x < grid.rows; ++x) {
            for (int z = 0; z < grid.columns; ++z) {
                number++;
                if (x != 0 || z != 0) {
                    auto generated_mat = renderer->materials().duplicate(mesh_instance.mesh->material(0), "material://generated" + std::to_string(number));
                    generated_mat->set_uniform("basecolor", glm::vec3{ 1, 0, 0 });
                    generated_mat->set_uniform("roughness", x / float(grid.rows - 1));
                    generated_mat->set_uniform("metalness", 1.0f - (z / float(grid.columns - 1)));

                    auto e = world_.create();
                    world_.add<sigma::transform>(e, txform.position + glm::vec3{ grid.row_spacing * x, 0, grid.column_spacing * z });
                    auto minst = world_.add<sigma::graphics::static_mesh_instance>(e, mesh_instance.mesh);
                    minst->materials[0] = generated_mat;
                }
            }
        }
    });

    // std::uniform_int_distribution<int> point_light_count_distribution_{ 0, 500 };
    // int number_of_point_lights = 512; //point_light_count_distribution_(generator_);
    // for (int i = 0; i < number_of_point_lights; ++i) {
    //     auto e = world_.create();
    //     random_transform(e);
    //     world_.add<sigma::graphics::point_light>(e, color_distribution_(generator_), scale_distribution_(generator_));
    // }

    // std::vector<sigma::resource::identifier> static_mesh_ids = {
    //     "static_mesh://water_packed:Torus",
    //     "static_mesh://water_packed:cube",
    //     "static_mesh://water_packed:piller.000",
    //     "static_mesh://water_packed:piller.001",
    //     "static_mesh://water_packed:piller.002",
    //     "static_mesh://water_packed:piller.003",
    //     "static_mesh://water_packed:piller.004",
    //     "static_mesh://water_packed:shape",
    //     "static_mesh://water_packed:sphere",
    //     "static_mesh://water_packed:suzan",
    // };
    //
    // std::uniform_int_distribution<int> static_mesh_count_distribution_{ 0, 1500 };
    // int number_of_static_meshes = 512; //static_mesh_count_distribution_(generator_);
    // std::uniform_int_distribution<std::size_t> static_mesh_distribution_{ 0, static_mesh_ids.size() - 1 };
    // for (int i = 0; i < number_of_static_meshes; ++i) {
    //     auto e = world_.create();
    //     auto& txform = random_transform(e);
    //     auto random_mesh_id = static_mesh_ids[static_mesh_distribution_(generator_)];
    //     world_.add<sigma::graphics::static_mesh_instance>(e, renderer_->static_meshes().get(random_mesh_id));
    // }
    //
    // std::cout << "Rendering " << number_of_point_lights << " point lights." << '\n';
    // std::cout << "Rendering " << number_of_static_meshes << " static meshes." << '\n';
}

sigma::transform& simple_game::random_transform(sigma::entity e)
{
    return *world_.add<sigma::transform>(e, position_distribution_(generator_),
        glm::quat{ rotation_distribution_(generator_) },
        glm::vec3{ scale_distribution_(generator_) });
}

void simple_game::update(std::chrono::duration<float> dt)
{
    static auto start = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::system_clock::now() - start);
    world_.for_each<sigma::transform, sigma::graphics::point_light>([elapsed, dt](sigma::entity e, sigma::transform& txform, const sigma::graphics::point_light& light) {
        txform.position.y += std::cos(elapsed.count()) * dt.count();
    });

    world_.for_each<sigma::transform>([&](sigma::entity e, sigma::transform& txform) {
        txform.matrix = txform.get_matrix();
    });

    world_.for_each<sigma::transform, sigma::graphics::directional_light>([&](sigma::entity e, const sigma::transform& txform, sigma::graphics::directional_light& light) {
        light.direction = glm::normalize(glm::vec3(txform.matrix * glm::vec4(0, 1, 0, 0)));
        //light.shadow_frustum.set_view(glm::lookAt(txform.position, txform.position - light.direction, glm::vec3(0, 1, 0)));
    });

    world_.for_each<sigma::transform, sigma::graphics::spot_light>([&](sigma::entity e, const sigma::transform& txform, sigma::graphics::spot_light& light) {
        light.direction = glm::normalize(glm::vec3(txform.matrix * glm::vec4(0, 1, 0, 0)));
        light.shadow_frustum.set_projection_view(2.5f * light.cutoff, 1.0f, 0.01f, 50.0f, glm::lookAt(txform.position, txform.position - light.direction, glm::vec3{ 0.0f, 1.0f, 0.0f }));
    });
}
