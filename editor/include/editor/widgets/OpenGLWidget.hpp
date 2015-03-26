#ifndef SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP

#include <editor/trackball_controller.hpp>

#include <game/scene.hpp>
#include <system/resource_manager.hpp>
#include <graphics/opengl/scene_renderer.hpp>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLWidget>

namespace sigmafive {
	namespace editor {
        namespace widgets {
            class OpenGLWidget: public QOpenGLWidget {
			public:
                explicit OpenGLWidget(system::resource_manager &resource_manager,game::scene &scene,
                                      QWidget *parent = nullptr);

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
                system::resource_manager &resource_manager_;
                game::scene &scene_;

                float4x4 view_matrix_;
                float4x4 projection_matrix_;
                trackball_controller trackball_controller_;

                std::unique_ptr<graphics::opengl::scene_renderer> scene_render_;
			};
		}
	}
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_OPENGLWIDGET_HPP
