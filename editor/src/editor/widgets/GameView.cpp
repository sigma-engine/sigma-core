#include <editor/widgets/GameView.hpp>

#include <QMatrix>
#include <QtQuick/QQuickWindow>
#include <QSGSimpleTextureNode>
#include <QOpenGLFramebufferObjectFormat>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            GameView::GameViewRenderer::GameViewRenderer(GameView *item,graphics::context_manager &context_manager)
                : needs_redraw_(false),
                  item_(item),
                  context_manager_(context_manager),
                  context_(context_manager_.create_context(CONTEXT_UID)) {
            }

            void GameView::GameViewRenderer::synchronize(QQuickFramebufferObject *item) {
                needs_redraw_ = true;

                color_ = item_->color();
                projection_matrix_  = float4x4::perspective(deg_to_rad(75.0f),float(item_->width())/float(item_->height()),0.01f,1000.0f);
                view_matrix_ = item_->trackball_controller_.matrix();

                auto entity_manager_ = item_->entityManager()->entity_manager();
                auto component_manager_ = item_->entityManager()->component_manager();
                auto component_system_manager_ = item_->entityManager()->component_system_manager();

                auto static_mesh_system_ = component_system_manager_->get_component_system<game::static_mesh_component_system>();

                context_manager_.make_current(context_.get());
                static_mesh_system_->init(&context_manager_);
                static_mesh_system_->process(*entity_manager_,*component_manager_);
            }

            QOpenGLFramebufferObject *GameView::GameViewRenderer::createFramebufferObject(const QSize &size) {
                QOpenGLFramebufferObjectFormat format;
                format.setSamples(8);
                format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
                return new QOpenGLFramebufferObject(size, format);
            }

            void GameView::GameViewRenderer::render() {
                if(needs_redraw_) {
                    context_->render(projection_matrix_,view_matrix_);

                    update();
                    item_->window()->resetOpenGLState();

                    needs_redraw_ = false;
                }
            }

            GameView::GameView(QQuickItem *parent)
                : QQuickFramebufferObject(parent),
                  engine_(*dynamic_cast<engine*>(qApp)),
                  trackball_controller_(.8f) {
            }

            QColor GameView::color() const {
                return color_;
            }

            void GameView::setColor(QColor color) {
                if(color_ != color) {
                    color_ = color;
                    emit colorChanged();
                }
            }

            void GameView::begin_rotate(QPoint pos) {
                trackball_controller_.begin_rotate(convert(pos));
            }

            void GameView::mouse_moved(QPoint pos) {
                trackball_controller_.update(convert(pos));
            }

            void GameView::end_rotate(QPoint pos) {
                trackball_controller_.end_rotate(convert(pos));
            }

            void GameView::wheel_scroll(int y) {
                trackball_controller_.zoom(y);
                this->update();
            }

            void GameView::begin_pan() {
                trackball_controller_.begin_pan();
            }

            void GameView::end_pan() {
                trackball_controller_.end_pan();
            }

            QQuickFramebufferObject::Renderer *GameView::createRenderer() const {
                return new GameViewRenderer{const_cast<GameView*>(this),engine_.graphics_context_manager()};
            }

        #if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
            QSGNode* GameView::updatePaintNode(QSGNode *node,QQuickItem::UpdatePaintNodeData *nodeData) {
                //This is a hack to fix the image being upside down because someone decided that was a nice
                //feature this is fixed in Qt 5.6
                //https://bugreports.qt.io/browse/QTBUG-41073?jql=project%20%3D%20QTBUG%20AND%20status%20%3D%20Closed%20AND%20fixVersion%20%3D%205.6%20AND%20text%20~%20%22texture%22%20ORDER%20BY%20priority%20DESC
                if (!node) {
                    node = QQuickFramebufferObject::updatePaintNode(node, nodeData);
                    QSGSimpleTextureNode *n = static_cast<QSGSimpleTextureNode *>(node);
                    if (n)
                        n->setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
                    return node;
                }

                return QQuickFramebufferObject::updatePaintNode(node, nodeData);
            }
        #endif //T_VERSION < QT_VERSION_CHECK(5, 6, 0)

            float2 GameView::convert(QPoint p) const {
                return float2( remap<float>(p.x(), 0.0f, width() , -1.0f, 1.0f),
                               -remap<float>(p.y(), 0.0f, height(), -1.0f, 1.0f));
            }
        }
    }
}
