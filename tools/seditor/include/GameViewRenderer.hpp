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
    float aspectRatio_;
    glm::ivec2 size_;
    std::shared_ptr<graphics::renderer> renderer_;

    GameView* item_;
};
}

#endif // SIGMA_GAME_VIEW_RENDERER_HPP
