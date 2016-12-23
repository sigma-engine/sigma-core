#include <sigma/context.hpp>
#include <sigma/game.hpp>

#include <iostream>

int main(int argc, char const* argv[])
{
    sigma::context ctx;

    ctx.load_plugin(argv[1]);
    ctx.set_renderer_class(argv[2]);

    ctx.load_plugin(argv[3]);
    ctx.set_game_class(argv[4]);

    std::chrono::duration<float> dt(1 / 60.0f);
    ctx.update(dt);
    return 0;
}
