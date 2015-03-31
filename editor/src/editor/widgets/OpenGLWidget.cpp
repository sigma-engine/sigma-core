#include <editor/widgets/OpenGLWidget.hpp>
#include <GL/gl.h>

namespace sigmafive {
	namespace editor {
		namespace widgets {
            OpenGLWidget::OpenGLWidget(system::resource_manager &resource_manager,game::scene &scene,QWidget *parent)
                : QOpenGLWidget(parent),
                  resource_manager_(resource_manager),
                  scene_(scene),
                  trackball_controller_(.8f) {

				QSurfaceFormat format;
				format.setDepthBufferSize(24);
				format.setSamples(8);
				setFormat(format);

				this->setFocus();
			}

			OpenGLWidget::~OpenGLWidget() {
			}

			void OpenGLWidget::initializeGL() {
                scene_render_ = std::unique_ptr<graphics::opengl::scene_renderer>(new graphics::opengl::scene_renderer(resource_manager_));
				QOpenGLWidget::initializeGL();
			}

			void OpenGLWidget::resizeGL(int w, int h) {
				trackball_controller_.resize(int2(w,h));
                projection_matrix_ = float4x4::perspective(deg_to_rad(45.0f),float(width())/float(height()),0.01f,1000.0f);

				gl::Viewport(0,0,width(),height());

                /*-gl::MatrixMode(GL_PROJECTION);
                gl::LoadIdentity();
                gl::MultMatrixf(&projection_matrix_[0].x);

                gl::MatrixMode(GL_MODELVIEW);
                gl::LoadIdentity();*/

				QOpenGLWidget::resizeGL(w,h);
			}

			void OpenGLWidget::paintGL() {
                view_matrix_ = trackball_controller_.matrix();

				gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
                gl::ClearColor(.75,.75,.75,1);

                /*glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glMultMatrixf(&view_matrix_[0].x);

                glColor3f(1.0, 1.0, 1.0);
                glBegin(GL_LINES);
                for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
                    glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
                    glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
                }
                glEnd();*/

				scene_render_->render(projection_matrix_,view_matrix_,scene_);

				QOpenGLWidget::paintGL();
			}

			void OpenGLWidget::mousePressEvent(QMouseEvent *e) {
				if(e->buttons() & Qt::MiddleButton) {
					trackball_controller_.mouse_down(int2(e->x(),e->y()));
					this->update();
				}
				QOpenGLWidget::mousePressEvent(e);
			}

			void OpenGLWidget::mouseMoveEvent(QMouseEvent *e) {
				if(e->buttons() & Qt::MiddleButton) {
					trackball_controller_.mouse_move(int2(e->x(),e->y()));
					this->update();
				}
			}

			void OpenGLWidget::mouseReleaseEvent(QMouseEvent *e) {
				if(e->buttons() & Qt::MiddleButton) {
					trackball_controller_.mouse_up(int2(e->x(),e->y()));
					this->update();
				}
			}

			void OpenGLWidget::wheelEvent(QWheelEvent *e) {
				trackball_controller_.mouse_scroll(e->delta());
				this->update();
				QOpenGLWidget::wheelEvent(e);
			}

			void OpenGLWidget::keyPressEvent(QKeyEvent *e) {
				QOpenGLWidget::keyPressEvent(e);
			}
		}
	}
}
