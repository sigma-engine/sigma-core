#include <GameViewRenderer.hpp>

#include <EditorContext.hpp>
#include <GameView.hpp>

#include <sigma/game.hpp>
#include <sigma/graphics/renderer.hpp>

#include <QOpenGLFramebufferObjectFormat>
#include <QtQuick/QQuickWindow>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>

#include <iostream>

namespace sigma {
GameViewRenderer::GameViewRenderer(EditorContext* ctx)
    : ctx_(ctx)
    , aspectRatio_(1)
    , renderer_(nullptr)
    , item_(nullptr)
{
}

QOpenGLFramebufferObject* GameViewRenderer::createFramebufferObject(const QSize& size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setSamples(8);
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    return new QOpenGLFramebufferObject(size, format);
}

void GameViewRenderer::synchronize(QQuickFramebufferObject* item)
{
    item_ = dynamic_cast<GameView*>(item);
    if (item_) {
        ctx_ = item_->activeContext();
        size_ = glm::ivec2{ item_->width(), item_->height() };
        aspectRatio_ = float(size_.x) / float(size_.y);
    }
}

void GameViewRenderer::render()
{
    if (item_) {
        if (ctx_) {
            if (renderer_ == nullptr)
                renderer_ = ctx_->create_renderer("sigma::opengl::renderer");
            auto g = ctx_->current_game();
            if (g) {
                glm::mat4 m = glm::mat4(1);
                //m = glm::translate(m, glm::vec3(0.0f, 0.0f, z));

                graphics::view_port vp{
                    g->entities,
                    g->transforms,
                    g->static_mesh_instances,
                    glm::perspective(0.785398f, aspectRatio_, 0.01f, 10000.0f),
                    m
                };
                renderer_->render(vp);
            }
        }
        item_->window()->resetOpenGLState();
    }
}
}
