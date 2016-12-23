#include <game_view.hpp>

#include <game_view_renderer.hpp>

#include <QSGSimpleTextureNode>

#include <iostream>

namespace sigma {
game_view::game_view(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
    , activeContext_(nullptr)
{
}

game_view::~game_view()
{
    activeContext_->release_renderer();
}

void game_view::releaseResources()
{
    activeContext_->release_renderer();
    QQuickFramebufferObject::releaseResources();
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
    return new game_view_renderer{};
}

QSGNode* game_view::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData* nodeData)
{
    //This is a hack to fix the image being upside down because someone decided that was a nice
    //feature this is fixed in Qt 5.6
    //https://bugreports.qt.io/browse/QTBUG-41073?jql=project%20%3D%20QTBUG%20AND%20status%20%3D%20Closed%20AND%20fixVersion%20%3D%205.6%20AND%20text%20~%20%22texture%22%20ORDER%20BY%20priority%20DESC
    if (!node) {
        node = QQuickFramebufferObject::updatePaintNode(node, nodeData);
        QSGSimpleTextureNode* n = static_cast<QSGSimpleTextureNode*>(node);
        if (n)
            n->setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
        return node;
    }

    return QQuickFramebufferObject::updatePaintNode(node, nodeData);
}
}
