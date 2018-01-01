#ifndef SIGMA_GAME_HPP
#define SIGMA_GAME_HPP

#include <sigma/blueprint.hpp>
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
    using component_set_type = typename World::component_set_type;
    using blueprint_type = blueprint<component_set_type>;

    game() = default;

    game(game<World>&&) = default;

    game<World>& operator=(game<World>&&) = default;

    virtual ~game() = default;

    World& world()
    {
        return world_;
    }

    void instantiate(const blueprint_type* blueprint)
    {
        for (const auto& entity : blueprint->entities) {
            auto e = world_.create();
            for (const auto& component : entity) {
                component_set_type::for_each([&](auto type_tag) {
                    using component_type = typename decltype(type_tag)::type;
                    if (const component_type* cmp = boost::get<component_type>(&component))
                        world_.template add<component_type>(e, *cmp);
                });
            }
        }
    }

    virtual void update(std::chrono::duration<float> dt) = 0;

protected:
    game(const game<World>&) = delete;
    game<World>& operator=(const game<World>&) = delete;

    World world_;
};
}

#endif // SIGMA_GAME_HPP
