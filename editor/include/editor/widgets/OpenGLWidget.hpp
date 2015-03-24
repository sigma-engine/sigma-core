#ifndef SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP

#include <editor/trackball_controller.hpp>

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

namespace sigmafive {
	namespace editor {
        namespace widgets {
			class OpenGLWidget: public QOpenGLWidget, public QOpenGLFunctions {
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
			private:
				float4x4 view_;
				float4x4 projection_;
				QMatrix4x4 projection;
				trackball_controller trackball_controller_;
			};
		}
	}
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
