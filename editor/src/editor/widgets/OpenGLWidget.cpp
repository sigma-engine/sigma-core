#include <editor/widgets/OpenGLWidget.hpp>

namespace sigmafive {
	namespace editor {
		namespace widgets {
			OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent), trackball_controller_(.8f) {
				QSurfaceFormat format;
				format.setDepthBufferSize(24);
				format.setSamples(8);
				setFormat(format);


				this->setFocus();
			}

			OpenGLWidget::~OpenGLWidget() {
			}

			void OpenGLWidget::initializeGL() {
				initializeOpenGLFunctions();

				glEnable(GL_DEPTH_TEST);
				glEnable(GL_POINT_SMOOTH);
				glPointSize(6);
				glLineWidth(2);

				QOpenGLWidget::initializeGL();
			}

			void OpenGLWidget::resizeGL(int w, int h) {
				trackball_controller_.resize(int2(w,h));

				glViewport(0,0,width(),height());

				//projection.setToIdentity();
				projection.perspective(45.0f,width()/float(height()),0.01f,1000.0f);
				projection_ = float4x4::perspective(deg_to_rad(45.0f),float(width())/float(height()),0.01f,1000.0f);

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glMultMatrixf(&projection_[0].x);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();

				QOpenGLWidget::resizeGL(w,h);
			}

			void OpenGLWidget::paintGL() {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glClearColor(.75,.75,.75,1);

				glLoadIdentity();
				view_ = trackball_controller_.matrix();
				glMultMatrixf(&view_[0].x);

				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_LINES);
				for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
					glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
					glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
				}
				glEnd();

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
					view_ = trackball_controller_.matrix();
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
