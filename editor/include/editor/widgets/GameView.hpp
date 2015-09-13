#ifndef SIGMAFIVE_EDITOR_WIDGETS_GAME_VIEW_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_GAME_VIEW_HPP

#include <QtQuick/QQuickFramebufferObject>

#include <editor/entity_manager_model.hpp>
#include <editor/trackball_controller.hpp>

#include <sigmafive/engine.hpp>

#include <sigmafive/graphics/context.hpp>

#include <sigmafive/game/entity_manager.hpp>
#include <sigmafive/game/component_manager.hpp>
#include <sigmafive/game/component_system_manager.hpp>
#include <sigmafive/game/static_mesh_component_system.hpp>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            class GameView : public QQuickFramebufferObject {
            Q_OBJECT
            public:
                class GameViewRenderer : public QQuickFramebufferObject::Renderer {
                public:
                    static constexpr const class_uid CONTEXT_UID = sigmafive::compile_time_hash(
                            "sigmafive::graphics::opengl::context");

                    GameViewRenderer(GameView *item, graphics::context_manager &context_manager);

                    void synchronize(QQuickFramebufferObject *item);

                    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);

                    void render();

                private:
                    bool needs_redraw_;

                    GameView *item_;

                    graphics::context_manager &context_manager_;
                    std::unique_ptr<sigmafive::graphics::context> context_;

                    QColor color_;
                    float4x4 view_matrix_;
                    float4x4 projection_matrix_;
                };

            public:
                Q_PROPERTY(QColor color
                                   READ
                                           color
                                   WRITE
                                           setColor
                                   NOTIFY
                                   colorChanged)
                Q_PROPERTY(entity_manager_model *entityManager
                                   READ
                                           entityManager
                                   WRITE
                                           setEntityManager
                                   NOTIFY
                                   entityManagerChanged)

                entity_manager_model *entityManager() {
                    return entity_manager_model_;
                }

                void setEntityManager(entity_manager_model *model) {
                    entity_manager_model_ = model;
                    emit entityManagerChanged();
                }

                GameView(QQuickItem *parent = 0);

                QColor color() const;

                void setColor(QColor color);

                Q_INVOKABLE void begin_rotate(QPoint pos);

                Q_INVOKABLE void mouse_moved(QPoint pos);

                Q_INVOKABLE void end_rotate(QPoint pos);

                Q_INVOKABLE void wheel_scroll(int y);

                Q_INVOKABLE void begin_pan();

                Q_INVOKABLE void end_pan();

                QQuickFramebufferObject::Renderer *createRenderer() const;

#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)

                QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *nodeData) override;

#endif
            signals:

                void colorChanged();

                void entityManagerChanged();

            private:
                friend class GameViewRenderer;

                float2 convert(QPoint p) const;

                engine &engine_;

                entity_manager_model *entity_manager_model_;

                QColor color_;
                editor::trackball_controller trackball_controller_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_GAME_VIEW_HPP
