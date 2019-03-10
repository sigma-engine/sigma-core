#ifndef SIGMA_GAME_HPP
#define SIGMA_GAME_HPP

#include <sigma/config.hpp>

#include <entt/entt.hpp>

#include <filesystem>
#include <fstream>

namespace sigma {

class game {
public:
    game() = default;

    game(game&&) = default;

    game& operator=(game&&) = default;

    virtual ~game() = default;

    entt::registry<> &registry() { return registry_; }

    const entt::registry<> &registry() const { return registry_; }

    virtual void update(std::chrono::duration<float> dt) = 0;
protected:
    game(const game&) = delete;
    game& operator=(const game&) = delete;

    entt::registry<> registry_;
};
}

#endif // SIGMA_GAME_HPP
