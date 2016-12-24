#include <qt_context.hpp>

#include <sigma/game.hpp>

namespace sigma {

qt_context::qt_context(QObject* parent)
    : QObject(parent)
{
    load_plugin("sopengl");
    load_plugin("simple-game");

    set_game_class("simple_game");
}
}
