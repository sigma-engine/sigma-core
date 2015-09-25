#ifndef SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP

#include <editor/config.hpp>
#include <QOpenGLWidget>

#include <editor/trackball_controller.hpp>

#include <sigmafive/object.hpp>

#include <cppbr/math/vec2.hpp>
#include <cppbr/math/mat4.hpp>

#include <QMouseEvent>
#include <QWheelEvent>

#include <memory>

namespace sigmafive {
    namespace graphics {
        class context;
        class context_manager;
    }
    namespace editor {

        class entity_manager;
        class component_manager;
        class component_system_manager;

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

                editor::component_manager *componentManager() const;

                void setComponentManager(editor::component_manager *component_manager);

                editor::component_system_manager *componentSystemManager() const;

                void setComponentSystemManager(editor::component_system_manager *component_system_manager);

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

                editor::entity_manager *managerModel_;
                editor::component_manager *component_manager_;
                editor::component_system_manager *component_system_manager_;
                graphics::context_manager *context_manager_;
                std::unique_ptr<sigmafive::graphics::context> context_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
