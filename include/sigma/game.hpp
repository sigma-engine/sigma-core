#ifndef SIGMA_GAME_HPP
#define SIGMA_GAME_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/util/json_conversion.hpp>
#include <sigma/world.hpp>

#include <json/json.h>

#include <boost/filesystem/path.hpp>

#include <fstream>

namespace sigma {

template <class World>
class game {
public:
    game() = default;

    game(game<World>&&) = default;

    game<World>& operator=(game<World>&&) = default;

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
    }

    virtual void update(std::chrono::duration<float> dt) = 0;

protected:
    game(const game<World>&) = delete;
    game<World>& operator=(const game<World>&) = delete;

    World world_;
};
}

#endif // SIGMA_GAME_HPP
