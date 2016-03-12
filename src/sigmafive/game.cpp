#include <sigmafive/game.hpp>

namespace sigmafive {

game::game(graphics::texture_cache& textures,
    graphics::shader_cache& shaders,
    graphics::material_cache& materials,
    graphics::static_mesh_cache& static_meshes,
    transform_manager& transforms,
    graphics::directional_light_manager& directional_lights,
    graphics::point_light_manager& point_lights,
    graphics::spot_light_manager& spot_lights,
    graphics::static_mesh_instance_manager& static_mesh_instances)
    : textures_(textures)
    , shaders_(shaders)
    , materials_(materials)
    , static_meshes_(static_meshes)
    , transforms_(transforms)
    , directional_lights_(directional_lights)
    , point_lights_(point_lights)
    , spot_lights_(spot_lights)
    , static_mesh_instances_(static_mesh_instances)
{
}

game::~game()
{
}

sigmafive::entity_manager& game::entities()
{
    return entities_;
}

const sigmafive::entity_manager& game::entities() const
{
    return entities_;
}

graphics::texture_cache& game::textures()
{
    return textures_;
}

const graphics::texture_cache& game::textures() const
{
    return textures_;
}

graphics::shader_cache& game::shaders()
{
    return shaders_;
}

const graphics::shader_cache& game::shaders() const
{
    return shaders_;
}

graphics::material_cache& game::materials()
{
    return materials_;
}

const graphics::material_cache& game::materials() const
{
    return materials_;
}

graphics::static_mesh_cache& game::static_meshes()
{
    return static_meshes_;
}

const graphics::static_mesh_cache& game::static_meshes() const
{
    return static_meshes_;
}

transform_manager& game::transforms()
{
    return transforms_;
}

const transform_manager& game::transforms() const
{
    return transforms_;
}

graphics::directional_light_manager& game::directional_lights()
{
    return directional_lights_;
}

const graphics::directional_light_manager& game::directional_lights() const
{
    return directional_lights_;
}

graphics::point_light_manager& game::point_lights()
{
    return point_lights_;
}

const graphics::point_light_manager& game::point_lights() const
{
    return point_lights_;
}

graphics::spot_light_manager& game::spot_lights()
{
    return spot_lights_;
}

const graphics::spot_light_manager& game::spot_lights() const
{
    return spot_lights_;
}

graphics::static_mesh_instance_manager& game::static_mesh_instances()
{
    return static_mesh_instances_;
}

const graphics::static_mesh_instance_manager& game::static_mesh_instances() const
{
    return static_mesh_instances_;
}
}
