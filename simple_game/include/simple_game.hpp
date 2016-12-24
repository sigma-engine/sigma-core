#ifndef SIMPLE_GAME_HPP
#define SIMPLE_GAME_HPP

#include <sigma/game.hpp>

class simple_game : public sigma::game {
public:
    simple_game(sigma::context* ctx);

    virtual void update(std::chrono::duration<float> dt) override;
};

#endif // SIMPLE_GAME_HPP
