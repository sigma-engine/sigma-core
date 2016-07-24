#include <sigma/game.hpp>

namespace sigma {

game::game(graphics::texture_cache& textures, graphics::shader_cache& shaders,
    graphics::material_cache& materials,
    graphics::static_mesh_cache& static_meshes,
    transform_manager& transforms,
    graphics::directional_light_manager& directional_lights,
    graphics::point_light_manager& point_lights,
    graphics::spot_light_manager& spot_lights,
    graphics::static_mesh_instance_manager& static_mesh_instances)
    : textures(textures)
    , shaders(shaders)
    , materials(materials)
    , static_meshes(static_meshes)
    , transforms(transforms)
    , directional_lights(directional_lights)
    , point_lights(point_lights)
    , spot_lights(spot_lights)
    , static_mesh_instances(static_mesh_instances)
{
}

game::~game() {}
}
