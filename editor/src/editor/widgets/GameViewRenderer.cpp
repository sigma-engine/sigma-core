#include <editor/widgets/GameViewRenderer.hpp>

#include <editor/widgets/GameView.hpp>

#include <sigmafive/game/static_mesh_component_system.hpp>

#include <QtQuick/QQuickWindow>
#include <QOpenGLFramebufferObjectFormat>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            GameViewRenderer::GameViewRenderer(GameView *item, graphics::context_manager *context_manager)
                : needs_redraw_(false),
                item_(item),
                context_manager_(context_manager),
                context_(context_manager_->create_context(CONTEXT_UID)) {
            }

            void GameViewRenderer::synchronize(QQuickFramebufferObject *item) {
                needs_redraw_ = true;
                projection_matrix_ = float4x4::perspective(deg_to_rad(75.0f),
                                                           float(item_->width()) / float(item_->height()), 0.01f,
                                                           1000.0f);
                view_matrix_ = item_->trackball_controller_.matrix();

                auto entity_manager_ = item_->entityManager();
                auto component_manager_ = item_->componentManager();
                auto component_system_manager_ = item_->componentSystemManager();

                auto static_mesh_system_ = component_system_manager_->get_component_system<game::static_mesh_component_system>();

                context_manager_->make_current(context_.get());
                static_mesh_system_->init(context_manager_);
                static_mesh_system_->process(entity_manager_, component_manager_);
            }

            QOpenGLFramebufferObject *GameViewRenderer::createFramebufferObject(const QSize &size) {
                context_->resize(uint2{size.width(),size.height()});
                QOpenGLFramebufferObjectFormat format;
                format.setSamples(8);
                format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
                return new QOpenGLFramebufferObject(size, format);
            }

            void GameViewRenderer::render() {
                if (needs_redraw_) {
                    context_->render(projection_matrix_, view_matrix_);

                    update();
                    item_->window()->resetOpenGLState();

                    needs_redraw_ = false;
                }
            }
        }
    }
}