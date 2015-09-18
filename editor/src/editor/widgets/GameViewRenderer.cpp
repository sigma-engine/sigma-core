#include <editor/widgets/GameViewRenderer.hpp>

#include <editor/entity_manager.hpp>
#include <editor/component_manager.hpp>
#include <editor/component_system_manager.hpp>

#include <editor/widgets/GameView.hpp>

#include <sigmafive/graphics/context.hpp>
#include <sigmafive/graphics/context_manager.hpp>

#include <sigmafive/game/static_mesh_component_system.hpp>

#include <QtQuick/QQuickWindow>
#include <QOpenGLFramebufferObjectFormat>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            GameViewRenderer::GameViewRenderer(graphics::context_manager *context_manager)
                : item_(nullptr),
                  context_(context_manager->create_context(CONTEXT_UID)) {
            }

            QOpenGLFramebufferObject *GameViewRenderer::createFramebufferObject(const QSize &size) {
                projection_matrix_ = float4x4::perspective(deg_to_rad(45.0f),
                                                           float(size.width()) / float(size.height()),
                                                           0.01f, 1000.0f);
                QOpenGLFramebufferObjectFormat format;
                format.setSamples(8);
                format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
                context_->resize(uint2{(unsigned int)size.width(),
                                       (unsigned int)size.height()});
                return new QOpenGLFramebufferObject(size, format);
            }

            void GameViewRenderer::synchronize(QQuickFramebufferObject *item) {
                item_ = static_cast<GameView*>(item);
                view_matrix_ = item_->viewMatrix();

                auto entity_manager_ = item_->entityManager();
                auto component_manager_ = item_->componentManager();
                auto component_system_manager_ = item_->componentSystemManager();

                auto static_mesh_system_ = component_system_manager_->get_component_system<game::static_mesh_component_system>();

                context_->make_current();
                static_mesh_system_->init(context_->context_manager());
                static_mesh_system_->process(entity_manager_, component_manager_);
            }

            void GameViewRenderer::render() {
                context_->render(projection_matrix_, view_matrix_);
                item_->window()->resetOpenGLState();
            }
        }
    }
}
