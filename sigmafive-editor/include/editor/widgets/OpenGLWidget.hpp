#ifndef SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP

#include <editor/trackball_controller.hpp>

#include <sigmafive/engine.hpp>
#include <sigmafive/game/scene.hpp>
#include <sigmafive/graphics/opengl/context.hpp>

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
                engine *engine_;
                game::scene *scene_;
            private:
                float2 convert(QPoint p) const;

                float4x4 view_matrix_;
                float4x4 projection_matrix_;
                trackball_controller trackball_controller_;

                std::unique_ptr<sigmafive::graphics::context> context_;
			};
		}
	}
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
