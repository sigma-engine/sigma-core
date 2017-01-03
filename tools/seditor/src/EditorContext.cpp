#include <EditorContext.hpp>

#include <sigma/game.hpp>

namespace sigma {

EditorContext::EditorContext(QObject* parent)
    : QObject(parent)
{
    load_plugin("sopengl");
    load_plugin("simple-game");
}

}
