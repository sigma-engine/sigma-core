#ifndef SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP

#include <editor/trackball_controller.hpp>

#include <sigmafive/engine.hpp>
#include <sigmafive/graphics/opengl/context.hpp>
#include <sigmafive/game/entity_manager.hpp>
#include <sigmafive/game/component_manager.hpp>
#include <sigmafive/game/component_system_manager.hpp>

#include <memory>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLWidget>

namespace sigmafive {
	namespace editor {
        namespace widgets {
            class OpenGLWidget: public QOpenGLWidget {
			public:
                explicit OpenGLWidget(QWidget *parent = nullptr);

				~OpenGLWidget();

				void initializeGL() override;

				void resizeGL(int w, int h) override;

				void paintGL() override;

				void mousePressEvent(QMouseEvent *e) override;

				void mouseMoveEvent(QMouseEvent *e) override;

				void mouseReleaseEvent(QMouseEvent *e) override;

				void wheelEvent(QWheelEvent *e) override;

				void keyPressEvent(QKeyEvent *e) override;

                void keyReleaseEvent(QKeyEvent *e) override;

                //TODO get rid of this
                game::entity_manager *entity_manager_;
                game::component_manager *component_manager_;
                game::component_system_manager *component_system_manager_;
            private:
                float2 convert(QPoint p) const;

                float4x4 view_matrix_;
                float4x4 projection_matrix_;
                trackball_controller trackball_controller_;

                graphics::context_manager *context_manager_;
                std::unique_ptr<sigmafive::graphics::context> context_;
			};
		}
	}
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
