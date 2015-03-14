#ifndef SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP

#include <QOpenGLWidget>

namespace sigmafive {
	namespace editor {
        namespace widgets {
			class OpenGLWidget: public QOpenGLWidget {
			public:
				explicit OpenGLWidget(QWidget *parent = nullptr);

				~OpenGLWidget();
			};
		}
	}
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
