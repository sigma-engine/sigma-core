#include <iostream>
#include <qt_context.hpp>

namespace sigma {

qt_context::qt_context(QObject* parent)
    : QObject(parent)
{
    load_plugin("../lib/sopengl");
    load_plugin("../lib/simple-game");
}

void qt_context::render()
{
    if (current_renderer == nullptr) {
        set_renderer_class("sigma::opengl::renderer");
        set_game_class("simple_game");
    }
    context::render();
}
}
