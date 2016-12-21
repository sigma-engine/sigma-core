#ifndef SIGMA_GAME_VIEW_RENDERER_HPP
#define SIGMA_GAME_VIEW_RENDERER_HPP

#include <QtQuick/QQuickFramebufferObject>

namespace sigma {
class game_view;
class qt_context;
class game_view_renderer : public QQuickFramebufferObject::Renderer {
public:
    game_view_renderer();

    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;

    void synchronize(QQuickFramebufferObject* item) override;

    void render() override;

private:
    game_view* item_;
    qt_context* ctx_;
};
}

#endif // SIGMA_GAME_VIEW_RENDERER_HPP
