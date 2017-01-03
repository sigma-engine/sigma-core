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
    , projectionMatrix_(1)
    , viewMatrix_(1)
    , renderer_(nullptr)
    , item_(nullptr)
{
}

QOpenGLFramebufferObject* GameViewRenderer::createFramebufferObject(const QSize& size)
{
    // Create the frame buffer object and bind it
    QOpenGLFramebufferObjectFormat format;
    //format.setSamples(8);
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    auto frameBuffer = new QOpenGLFramebufferObject(size, format);
    frameBuffer->bind();

    game_ = nullptr; // Kill the game first

    //Then create the renderer
    size_ = glm::vec2{ size.width(), size.height() };
    renderer_ = ctx_->create_renderer("sigma::opengl::renderer", size_);
    game_ = ctx_->create_game("simple_game", renderer_);

    // Setup the viewport projection
    // TODO do not hard code z near, far and fov
    projectionMatrix_ = glm::perspective(0.785398f, float(size.width()) / float(size.height()), 0.01f, 10000.0f);

    return frameBuffer;
}

void GameViewRenderer::synchronize(QQuickFramebufferObject* item)
{
    item_ = dynamic_cast<GameView*>(item);
    if (item_) {
        ctx_ = item_->activeContext();
        auto ctrl = item_->controller();
        if (ctrl)
            viewMatrix_ = ctrl->matrix();
    }
}

void GameViewRenderer::render()
{
    if (item_ && renderer_ && game_) {
        graphics::view_port vp{
            game_->entities,
            game_->transforms,
            game_->static_mesh_instances,
            game_->point_lights,
            game_->directional_lights,
            game_->spot_lights,
            projectionMatrix_,
            viewMatrix_,
            0.01f, 10000.0f, // TODO do not hard code z near and far
            size_
        };
        renderer_->render(vp);
        item_->window()->resetOpenGLState();
    }
}
}
