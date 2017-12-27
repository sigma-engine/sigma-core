#ifndef SIMPLE_WORLD_HPP
#define SIMPLE_WORLD_HPP

#include <grid_component.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/transform.hpp>
#include <sigma/world.hpp>

using simple_world = sigma::world<sigma::transform,
    sigma::graphics::directional_light,
    sigma::graphics::point_light,
    sigma::graphics::spot_light,
    sigma::graphics::static_mesh_instance,
    grid_component>;

#endif // SIMPLE_WORLD_HPP
