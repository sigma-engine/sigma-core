#ifndef SIGMA_GAME_HPP
#define SIGMA_GAME_HPP

#include <sigma/config.hpp>
#include <sigma/blueprint.hpp>

#include <entt/entt.hpp>

namespace sigma {

class game {
public:
    game() = default;

    game(game&&) = default;

    game& operator=(game&&) = default;

    virtual ~game() = default;

    virtual void update(std::chrono::duration<float> dt) = 0;

    entt::registry<> registry;
protected:
    game(const game&) = delete;
    game& operator=(const game&) = delete;
};
}

#endif // SIGMA_GAME_HPP
