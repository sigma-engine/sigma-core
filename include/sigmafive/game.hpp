#ifndef SIGMAFIVE_ENGINE_GAME_HPP
#define SIGMAFIVE_ENGINE_GAME_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/transform.hpp>
#include <sigmafive/entity_manager.hpp>

#include <sigmafive/transform.hpp>

#include <sigmafive/graphics/shader.hpp>
#include <sigmafive/graphics/texture.hpp>
#include <sigmafive/graphics/material.hpp>
#include <sigmafive/graphics/spot_light.hpp>
#include <sigmafive/graphics/static_mesh.hpp>
#include <sigmafive/graphics/point_light.hpp>
#include <sigmafive/graphics/directional_light.hpp>
#include <sigmafive/graphics/static_mesh_instance.hpp>

#include <chrono>

namespace sigmafive {
    class SIGMAFIVE_API game {
    public:
        game(graphics::texture_cache &textures,
             graphics::shader_cache &shaders,
             graphics::material_cache &materials,
             graphics::static_mesh_cache &static_meshes,
             transform_manager &transforms,
             graphics::directional_light_manager &directional_lights,
             graphics::point_light_manager &point_lights,
             graphics::spot_light_manager &spot_lights,
             graphics::static_mesh_instance_manager &static_mesh_instances);

        virtual ~game();

        sigmafive::entity_manager &entity_manager();

        const sigmafive::entity_manager &entity_manager() const;

        graphics::texture_cache &textures();

        const graphics::texture_cache &textures() const;

        graphics::shader_cache &shaders();

        const graphics::shader_cache &shaders() const;

        graphics::material_cache &materials();

        const graphics::material_cache &materials() const;

        graphics::static_mesh_cache &static_meshes();

        const graphics::static_mesh_cache &static_meshes() const;

        transform_manager &transforms();

        const transform_manager &transforms() const;

        graphics::directional_light_manager &directional_lights();

        const graphics::directional_light_manager &directional_lights() const;

        graphics::point_light_manager &point_lights();

        const graphics::point_light_manager &point_lights() const;

        graphics::spot_light_manager &spot_lights();

        const graphics::spot_light_manager &spot_lights() const;

        graphics::static_mesh_instance_manager &static_mesh_instances();

        const graphics::static_mesh_instance_manager &static_mesh_instances() const;

        virtual void update(std::chrono::duration<float> dt) = 0;
    private:
        sigmafive::entity_manager entity_manager_;

        graphics::texture_cache &textures_;
        graphics::shader_cache &shaders_;
        graphics::material_cache &materials_;
        graphics::static_mesh_cache &static_meshes_;
        transform_manager &transforms_;
        graphics::directional_light_manager &directional_lights_;
        graphics::point_light_manager &point_lights_;
        graphics::spot_light_manager &spot_lights_;
        graphics::static_mesh_instance_manager &static_mesh_instances_;
    };
}

#endif //SIGMAFIVE_ENGINE_GAME_HPP
