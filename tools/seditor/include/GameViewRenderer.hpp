#ifndef SIGMA_GAME_VIEW_RENDERER_HPP
#define SIGMA_GAME_VIEW_RENDERER_HPP

#include <sigma/graphics/renderer.hpp>

#include <QtQuick/QQuickFramebufferObject>

#include <glm/vec2.hpp>

#include <memory>

namespace sigma {
class GameView;
class EditorContext;
class GameViewRenderer : public QQuickFramebufferObject::Renderer {
public:
    GameViewRenderer(EditorContext* ctx);

    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;

    void synchronize(QQuickFramebufferObject* item) override;

    void render() override;

private:
    EditorContext* ctx_;

    glm::mat4 projectionMatrix_;
    glm::mat4 viewMatrix_;
    std::shared_ptr<graphics::renderer> renderer_;

    GameView* item_;
    glm::vec2 size_;
};
}

#endif // SIGMA_GAME_VIEW_RENDERER_HPP
