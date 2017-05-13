#ifndef SIGMA_GAME_HPP
#define SIGMA_GAME_HPP

#include <sigma/entity.hpp>
#include <sigma/graphics/renderer.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>

#include <json/json.h>

#include <boost/filesystem/path.hpp>

#include <fstream>

namespace sigma {

template <class World>
class game {
public:
    game(graphics::renderer* renderer)
        : renderer_(renderer)
    {
    }

    virtual ~game() = default;

    World& world()
    {
        return world_;
    }

    void load(boost::filesystem::path file_path)
    {
        std::ifstream file{ file_path.string(), std::ios::in };
        Json::Value scene_data;
        file >> scene_data;
        json::from_json(scene_data, world_);

        world_.template for_each<sigma::graphics::static_mesh_instance>([&](sigma::entity e, sigma::graphics::static_mesh_instance& mesh_instance) {
            mesh_instance.mesh.set_manager(&renderer_->static_meshes());
        });
    }

    virtual void update(std::chrono::duration<float> dt) = 0;

protected:
    World world_;
    graphics::renderer* renderer_;
};
}

#endif // SIGMA_GAME_HPP
