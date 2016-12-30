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
    //this->setMirrorVertically(true);
}

GameView::~GameView()
{
}

QQuickFramebufferObject::Renderer* GameView::createRenderer() const
{
    return new GameViewRenderer{ activeContext_ };
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

TrackballController* GameView::controller() const
{
    return controller_;
}

void GameView::setController(TrackballController* ctl)
{
    if (controller_ != ctl) {
        controller_ = ctl;
        emit controllerChanged(ctl);
    }
}
}
