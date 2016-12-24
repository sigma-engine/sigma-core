#include <game_view.hpp>

#include <QQuickWindow>
#include <QRunnable>
#include <QSGSimpleTextureNode>
#include <game_view_renderer.hpp>
#include <iostream>

namespace sigma {
game_view::game_view(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
    , activeContext_(nullptr)
{
    this->setMirrorVertically(true);
}

game_view::~game_view()
{
}

qt_context* game_view::activeContext()
{
    return activeContext_;
}

void game_view::setActiveContext(qt_context* ctx)
{
    if (activeContext_ != ctx) {
        activeContext_ = ctx;
        emit activeContextChanged(activeContext_);
    }
}

QQuickFramebufferObject::Renderer* game_view::createRenderer() const
{
    return new game_view_renderer{ activeContext_ };
}
}
