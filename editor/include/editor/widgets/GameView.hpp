#ifndef SIGMAFIVE_EDITOR_WIDGETS_GAME_VIEW_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_GAME_VIEW_HPP

#include <editor/config.hpp>
#include <QtQuick/QQuickFramebufferObject>

#include <editor/component_manager.hpp>
#include <editor/entity_manager.hpp>
#include <editor/trackball_controller.hpp>
#include <editor/component_system_manager.hpp>

#include <sigmafive/engine.hpp>
#include <sigmafive/graphics/context.hpp>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            class EDITOR_API GameView : public QQuickFramebufferObject {
            Q_OBJECT
            public:
                class GameViewRenderer : public QQuickFramebufferObject::Renderer {
                public:
                    static constexpr const class_uid CONTEXT_UID = sigmafive::compile_time_hash(
                            "sigmafive::graphics::opengl::context");

                    GameViewRenderer(GameView *item, graphics::context_manager *context_manager);

                    void synchronize(QQuickFramebufferObject *item);

                    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);

                    void render();
                private:
                    bool needs_redraw_;

                    GameView *item_;

                    graphics::context_manager *context_manager_;
                    std::unique_ptr<sigmafive::graphics::context> context_;

                    float4x4 view_matrix_;
                    float4x4 projection_matrix_;
                };

            public:
                Q_PROPERTY(entity_manager *entityManager READ entityManager WRITE setEntityManager NOTIFY entityManagerChanged);
                Q_PROPERTY(component_manager *componentManager READ componentManager WRITE setComponentManager NOTIFY componentManagerChanged);
                Q_PROPERTY(component_system_manager *componentSystemManager READ componentSystemManager WRITE setComponentSystemManager NOTIFY componentSystemManagerChanged);

                GameView(QQuickItem *parent = 0);

                entity_manager *entityManager();

                void setEntityManager(entity_manager *model);

                component_manager * componentManager() const;

                void setComponentManager(component_manager *componentManager);

                component_system_manager * componentSystemManager() const;

                void setComponentSystemManager(component_system_manager *componentSystemManager);

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

                void entityManagerChanged();

                void componentManagerChanged();

                void componentSystemManagerChanged();

            private:
                friend class GameViewRenderer;

                float2 convert(QPoint p) const;

                engine &engine_;

                entity_manager *entityManager_;
                editor::component_manager *componentManager_;
                editor::component_system_manager *componentSystemManager_;

                editor::trackball_controller trackball_controller_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_GAME_VIEW_HPP
