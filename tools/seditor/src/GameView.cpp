#include <GameView.hpp>

#include <GameViewRenderer.hpp>

#include <QQuickWindow>
#include <QRunnable>
#include <QSGSimpleTextureNode>

#include <iostream>

namespace sigma {
GameView::GameView(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
    , activeContext_(nullptr)
{
    this->setMirrorVertically(true);
}

GameView::~GameView()
{
}

EditorContext* GameView::activeContext()
{
    return activeContext_;
}

void GameView::setActiveContext(EditorContext* ctx)
{
    if (activeContext_ != ctx) {
        activeContext_ = ctx;
        emit activeContextChanged(activeContext_);
    }
}

QQuickFramebufferObject::Renderer* GameView::createRenderer() const
{
    return new GameViewRenderer{ activeContext_ };
}
}
