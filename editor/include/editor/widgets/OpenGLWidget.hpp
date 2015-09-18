#ifndef SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP

#include <editor/config.hpp>

#include <QOpenGLWidget>

#include <editor/entity_manager.hpp>
#include <editor/component_manager.hpp>
#include <editor/trackball_controller.hpp>
#include <editor/component_system_manager.hpp>

#include <sigmafive/graphics/context_manager.hpp>

#include <memory>
#include <QMouseEvent>
#include <QWheelEvent>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            class EDITOR_API OpenGLWidget : public QOpenGLWidget {
            Q_OBJECT
            public:
                static constexpr const class_uid CONTEXT_UID = sigmafive::compile_time_hash(
                        "sigmafive::graphics::opengl::context");

                explicit OpenGLWidget(QWidget *parent = nullptr);

                ~OpenGLWidget();

                entity_manager *entityManager();

                void setEntityManager(entity_manager *model);

                game::component_manager *componentManager() const;

                void setComponentManager(game::component_manager *component_manager);

                game::component_system_manager *componentSystemManager() const;

                void setComponentSystemManager(game::component_system_manager *component_system_manager);

                graphics::context_manager * contextManager() const;

                void setContextManager(graphics::context_manager *context_manager);

                void initializeGL() override;

                void resizeGL(int w, int h) override;

                void paintGL() override;

                void mousePressEvent(QMouseEvent *e) override;

                void mouseMoveEvent(QMouseEvent *e) override;

                void mouseReleaseEvent(QMouseEvent *e) override;

                void wheelEvent(QWheelEvent *e) override;

                void keyPressEvent(QKeyEvent *e) override;

                void keyReleaseEvent(QKeyEvent *e) override;

            signals:

                void entityManagerChanged();

            private:
                float2 convert(QPoint p) const;

                float4x4 view_matrix_;
                float4x4 projection_matrix_;
                trackball_controller trackball_controller_;

                entity_manager *managerModel_;
                game::component_manager *component_manager_;
                game::component_system_manager *component_system_manager_;
                graphics::context_manager *context_manager_;
                std::unique_ptr<sigmafive::graphics::context> context_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
