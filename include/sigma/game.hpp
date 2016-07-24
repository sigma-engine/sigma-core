#ifndef SIGMA_ENGINE_GAME_HPP
#define SIGMA_ENGINE_GAME_HPP

#include <sigma/config.hpp>

#include <chrono>

#include <sigma/entity_manager.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/reflect/reflect.hpp>
#include <sigma/transform.hpp>

namespace sigma {

RCLASS()
class sigma_API game {
public:
    game(graphics::texture_cache& textures, graphics::shader_cache& shaders,
        graphics::material_cache& materials,
        graphics::static_mesh_cache& static_meshes,
        transform_manager& transforms,
        graphics::directional_light_manager& directional_lights,
        graphics::point_light_manager& point_lights,
        graphics::spot_light_manager& spot_lights,
        graphics::static_mesh_instance_manager& static_mesh_instances);

    virtual ~game();

    RPROPERTY()
    sigma::entity_manager entities;

    RPROPERTY()
    graphics::texture_cache& textures;

    RPROPERTY()
    graphics::shader_cache& shaders;

    RPROPERTY()
    graphics::material_cache& materials;

    RPROPERTY()
    graphics::static_mesh_cache& static_meshes;

    RPROPERTY()
    transform_manager& transforms;

    RPROPERTY()
    graphics::directional_light_manager& directional_lights;

    RPROPERTY()
    graphics::point_light_manager& point_lights;

    RPROPERTY()
    graphics::spot_light_manager& spot_lights;

    RPROPERTY()
    graphics::static_mesh_instance_manager& static_mesh_instances;

    virtual void update(std::chrono::duration<float> dt) = 0;
};
}

#endif // SIGMA_ENGINE_GAME_HPP
