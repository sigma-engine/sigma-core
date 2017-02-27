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
    load("../data/material_test_scene.scn");
    // load("../data/water_packed.scn");

    int number = 0;
    int MAX = 5;
    for (int x = 0; x < MAX; ++x) {
        for (int z = 0; z < MAX; ++z) {
            number++;
            auto material_ball = renderer->static_meshes().duplicate("static_mesh://material_ball:material_ball", "static_mesh://material_ball:material_ball" + std::to_string(number));
            auto generated_mat = renderer->materials().duplicate("material://yellow_plastic", "material://generated" + std::to_string(number));
            material_ball->set_material(0, generated_mat);
            generated_mat->set_uniform("basecolor", glm::vec3{ 1, 1, 1 });
            generated_mat->set_uniform("roughness", x / float(MAX - 1));
            generated_mat->set_uniform("metalness", 1.0f - (z / float(MAX - 1)));

            auto e = world_.create();
            world_.add<sigma::transform>(e, glm::vec3{ 1.5f * x, 0, 1.5f * z });
            world_.add<sigma::graphics::static_mesh_instance>(e, material_ball);
        }
    }

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
    world_.for_each<sigma::transform, sigma::graphics::point_light>([dt](sigma::entity e, sigma::transform& txform, const sigma::graphics::point_light& light) {
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::system_clock::now() - start);
        auto pos = txform.position();
        pos.y += std::cos(elapsed.count()) * dt.count();
        txform.set_position(pos);
    });
}
