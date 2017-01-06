#include "simple_game.hpp"

#include <glm/vec3.hpp>

#include <boost/math/constants/constants.hpp>

#include <chrono>

simple_game::simple_game(sigma::graphics::renderer* renderer)
    : sigma::game(renderer)
    , generator_(std::default_random_engine::default_seed)
    , position_distribution_{ { -40, 0, -40 }, { 40.0f, 2.5, 40.0f } }
    , rotation_distribution_{ glm::vec3{ 0.0f }, glm::vec3{ 2.0f * boost::math::constants::pi<float>() } }
    , scale_distribution_{ 0.1f, 20.0f }
    , color_distribution_{ glm::vec3{ 0.0f }, glm::vec3{ 1.0f } }
{
    load("../data/water_packed.scn");

    std::uniform_int_distribution<int> count_distribution_{ 0, 1500 };

    int number_of_point_lights = count_distribution_(generator_);
    std::cout << "Rendering " << number_of_point_lights << " point lights." << std::endl;
    for (int i = 0; i < number_of_point_lights; ++i) {
        auto e = entities.create();
        auto& txform = random_trasnform(e);
        auto& light = point_lights.add(e);
        light.color = color_distribution_(generator_);
        light.intensity = scale_distribution_(generator_);
    }
}

sigma::transform& simple_game::random_trasnform(sigma::entity e)
{
    auto& txform = transforms.add(e);
    txform.set_position(position_distribution_(generator_));
    txform.set_rotation(glm::quat{ rotation_distribution_(generator_) });
    txform.set_scale(glm::vec3{ scale_distribution_(generator_) });
    return txform;
}

void simple_game::update(std::chrono::duration<float> dt)
{
    std::cout << " this is a test " << std::endl;
}

SIGMA_EXPORT_GAME_CLASSES(simple_game)
