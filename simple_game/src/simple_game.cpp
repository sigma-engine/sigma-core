#include "simple_game.hpp"

#include <iostream>

simple_game::simple_game(sigma::context* ctx)
    : sigma::game(ctx)
{
    load("/home/aaron/Desktop/sigma-engine/simple_game/resources/water_packed.scn");
    /*const sigma::resource::identifier crate001_mesh{ "static_mesh://floor:Plane" };
    const sigma::resource::identifier suzanne_mesh{ "static_mesh://test_folder/suzanne:Suzanne" };
    const sigma::resource::identifier sky_dome{ "static_mesh://test_folder/sky_dome:sky_dome" };*/
    const sigma::resource::identifier ground{ "static_mesh://water_packed:ground" };

    /*{
        auto test = entities.create();
        auto& txform = transforms.add(test);
        static_mesh_instances.add(test, crate001_mesh);
    }
    {
        auto e = entities.create();
        transforms.add(e);
        static_mesh_instances.add(e, suzanne_mesh);
}
    {
        auto e = entities.create();
        transforms.add(e);
        static_mesh_instances.add(e, sky_dome);
    }
    {
        auto e = entities.create();
        transforms.add(e);
        static_mesh_instances.add(e, ground);
    }
    auto e = entities.create();
    auto& txf = transforms.add(e);
    txf.scale = glm::vec3(20);
    auto& l = directional_lights.add(e);
    l.intensity = 2;*/
}

void simple_game::update(std::chrono::duration<float> dt)
{
    std::cout << " this is a test " << std::endl;
}

SIGMA_EXPORT_GAME_CLASSES(simple_game)
