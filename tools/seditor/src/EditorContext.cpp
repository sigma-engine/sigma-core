#include <EditorContext.hpp>

#include <sigma/game.hpp>

namespace sigma {

EditorContext::EditorContext(QObject* parent)
    : QObject(parent)
{
    load_plugin("sopengl");
    load_plugin("simple-game");

    set_game_class("simple_game");
}
}
