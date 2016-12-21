#include <game_view_renderer.hpp>

#include <game_view.hpp>
#include <qt_context.hpp>

#include <QOpenGLFramebufferObjectFormat>
#include <QtQuick/QQuickWindow>

namespace sigma {
game_view_renderer::game_view_renderer()
    : item_(nullptr)
    , ctx_(nullptr)
{
}

QOpenGLFramebufferObject* game_view_renderer::createFramebufferObject(const QSize& size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setSamples(8);
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    return new QOpenGLFramebufferObject(size, format);
}

void game_view_renderer::synchronize(QQuickFramebufferObject* item)
{
    item_ = dynamic_cast<game_view*>(item);
    if (item_)
        ctx_ = item_->activeContext();
}

void game_view_renderer::render()
{
    if (ctx_)
        ctx_->render();
    item_->window()->resetOpenGLState();
}
}
