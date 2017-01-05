#include "simple_game.hpp"

#include <glm/vec3.hpp>

#include <boost/math/constants/constants.hpp>

simple_game::simple_game(sigma::graphics::renderer* renderer)
    : sigma::game(renderer)
	, position_distribution_{{-10,-10,0}, { 10.0f,10.0f, 2.5f}}
	, scale_distribution_{ glm::vec3 { 5.0f }, glm::vec3{ 100.0f } }
	, rotation_distribution_{ glm::vec3{ 0.0f }, glm::vec3{ 2.0f * boost::math::constants::pi<float>() } }
{
    load("../data/water_packed.scn");
	/*std::uniform_int_distribution<int> count_distribution_{100,200};
	int c = count_distribution_(generator_);
	std::cout << c << std::endl;
	for (int i = 0; i < c;++i) {
		auto e = entities.create();
		auto &txform = random_trasnform(e);
		auto &light = point_lights.add(e);
		light.intensity = 1;
	}*/
}

sigma::transform &simple_game::random_trasnform(sigma::entity e)
{
	auto &txform = transforms.add(e);
	txform.set_position(position_distribution_(generator_));
	txform.set_rotation(glm::quat{ rotation_distribution_(generator_) });
	txform.set_scale(scale_distribution_(generator_));
	return txform;
}

void simple_game::update(std::chrono::duration<float> dt)
{
    std::cout << " this is a test " << std::endl;
}

SIGMA_EXPORT_GAME_CLASSES(simple_game)
