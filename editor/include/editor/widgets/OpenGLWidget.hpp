#ifndef SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP

#include <editor/config.hpp>

#include <QOpenGLWidget>

#include <editor/trackball_controller.hpp>
#include <editor/entity_manager_model.hpp>

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

                entity_manager_model *entityManager() {
                    return entity_manager_model_;
                }

                void setEntityManager(entity_manager_model *model) {
                    entity_manager_model_ = model;
                    emit entityManagerChanged();
                }

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

                entity_manager_model *entity_manager_model_;
                std::unique_ptr<sigmafive::graphics::context> context_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
