#include <editor/widgets/GameView.hpp>

#include <editor/entity_manager.hpp>
#include <editor/component_manager.hpp>
#include <editor/component_system_manager.hpp>
#include <editor/widgets/GameViewRenderer.hpp>

#include <sigmafive/engine.hpp>

#include <QSGSimpleTextureNode>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            GameView::GameView(QQuickItem *parent)
                    : QQuickFramebufferObject(parent),
                      trackball_controller_(.8f) {
            }

            entity_manager *GameView::entityManager() {
                return entityManager_;
            }

            void GameView::setEntityManager(entity_manager *model) {
                entityManager_ = model;
                emit entityManagerChanged();
            }

            component_manager *GameView::componentManager() const {
                return componentManager_;
            }

            void GameView::setComponentManager(component_manager *componentManager) {
                componentManager_ = componentManager;
                emit componentManagerChanged();
            }

            component_system_manager *GameView::componentSystemManager() const {
                return componentSystemManager_;
            }

            void GameView::setComponentSystemManager(component_system_manager *componentSystemManager) {
                componentSystemManager_ = componentSystemManager;
                emit componentSystemManagerChanged();
            }

            float4x4 GameView::viewMatrix() const {
                return trackball_controller_.matrix();
            }

            void GameView::begin_rotate(QPoint pos) {
                trackball_controller_.begin_rotate(convert(pos));
            }

            void GameView::mouse_moved(QPoint pos) {
                trackball_controller_.update(convert(pos));
                emit viewMatrixChanged();
                this->update();
            }

            void GameView::end_rotate(QPoint pos) {
                trackball_controller_.end_rotate(convert(pos));
            }

            void GameView::wheel_scroll(int y) {
                trackball_controller_.zoom(y);
                emit viewMatrixChanged();
                this->update();
            }

            void GameView::begin_pan() {
                trackball_controller_.begin_pan();
            }

            void GameView::end_pan() {
                trackball_controller_.end_pan();
            }

            QQuickFramebufferObject::Renderer *GameView::createRenderer() const {
                //TODO is there a better way to do this??
                return new GameViewRenderer{dynamic_cast<engine *>(qApp)->graphics_context_manager()};
            }

#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
            QSGNode *GameView::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *nodeData) {
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
                return float2(remap<float>(p.x(), 0.0f, width(), -1.0f, 1.0f),
                              -remap<float>(p.y(), 0.0f, height(), -1.0f, 1.0f));
            }
        }
    }
}
