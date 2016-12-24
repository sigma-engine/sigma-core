#ifndef SIGMA_GAME_VIEW_RENDERER_HPP
#define SIGMA_GAME_VIEW_RENDERER_HPP

#include <QtQuick/QQuickFramebufferObject>
#include <glm/vec2.hpp>
#include <memory>
#include <sigma/graphics/renderer.hpp>

namespace sigma {
class game_view;
class qt_context;
class game_view_renderer : public QQuickFramebufferObject::Renderer {
public:
    game_view_renderer(qt_context *ctx);

    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;

    void synchronize(QQuickFramebufferObject* item) override;

    void render() override;

private:
	qt_context* ctx_;
	float aspectRatio_;
	glm::ivec2 size_;
	std::shared_ptr<graphics::renderer> renderer_;

	game_view* item_;
};
}

#endif // SIGMA_GAME_VIEW_RENDERER_HPP
