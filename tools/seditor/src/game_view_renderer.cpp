#include <game_view_renderer.hpp>

#include <game_view.hpp>
#include <qt_context.hpp>

#include <glm/vec2.hpp>

#include <QOpenGLFramebufferObjectFormat>
#include <QtQuick/QQuickWindow>
#include <iostream>
namespace sigma {
game_view_renderer::game_view_renderer()
    : item_(nullptr)
    , ctx_(nullptr)
    , size_(0, 0)
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
    if (item_) {
        ctx_ = item_->activeContext();
        size_ = glm::ivec2{ item_->width(), item_->height() };
    }
}

void game_view_renderer::render()
{
    if (item_) {
        if (ctx_)
            ctx_->render(size_);
        item_->window()->resetOpenGLState();
    }
}
}
