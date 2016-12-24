#include <game_view_renderer.hpp>

#include <game_view.hpp>
#include <qt_context.hpp>
#include <sigma/graphics/renderer.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sigma/game.hpp>

#include <glm/vec2.hpp>

#include <QOpenGLFramebufferObjectFormat>
#include <QtQuick/QQuickWindow>
#include <iostream>
namespace sigma {
game_view_renderer::game_view_renderer(qt_context *ctx)
    : ctx_(ctx)
	, aspectRatio_(1)
	, renderer_(nullptr)
	, item_(nullptr)
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
		size_ = glm::ivec2{item_->width(),item_->height()};
		aspectRatio_ = float(size_.x) / float(size_.y);
	}
}

void game_view_renderer::render()
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
					glm::perspective(0.785398f,aspectRatio_,0.01f,10000.0f),
					m
				};
				renderer_->render(vp);
			}
		}
		item_->window()->resetOpenGLState();
	}
}
}
