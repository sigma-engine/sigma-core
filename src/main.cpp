#include <sigma/Algorithm.hpp>

#include <SDL2/SDL.h>
#include <SimpleGame.hpp>

#include <string>
#include <vector>

int main(int argc, char** argv)
{
    std::vector<std::string> arguments(argc - 1);
    std::transform(argv + 1, argv + (argc - 1) + 1, arguments.begin(), [](auto arg) { return arg; });
    std::stable_sort(arguments.begin(), arguments.end(), [](auto a, auto b) {
        return StartsWith(a, "-") && !StartsWith(b, "-");
    });

    SimpleGame game;
    return game.run(arguments);
}
