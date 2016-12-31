#include <EditorContext.hpp>

#include <sigma/game.hpp>

namespace sigma {

EditorContext::EditorContext(QObject* parent)
    : QObject(parent)
{
    load_plugin("sopengl");
    load_plugin("simple-game");

    current_game_ = create_game("simple_game");
}

std::shared_ptr<game> EditorContext::current_game() const
{
    return current_game_;
}
}
