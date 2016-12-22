#include <iostream>
#include <qt_context.hpp>

namespace sigma {

qt_context::qt_context(QObject* parent)
    : QObject(parent)
{
    load_plugin("../bin/sopengl");
    load_plugin("../bin/simple-game");
}

void qt_context::render(glm::ivec2 size)
{
    if (current_renderer == nullptr) {
        current_game = nullptr;
        current_renderer = nullptr;
        set_renderer_class("sigma::opengl::renderer");
        set_game_class("simple_game");
    }
    context::render(size);
}

void qt_context::release_renderer()
{
    current_game = nullptr;
    current_renderer = nullptr;
}
}
