#include <editor/widgets/OpenGLWidget.hpp>
#include <QMatrix4x4>

namespace sigmafive {
	namespace editor {
		namespace widgets {
            OpenGLWidget::OpenGLWidget(QWidget *parent)
                : QOpenGLWidget(parent),
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
                scene_render_ = std::unique_ptr<graphics::opengl::scene_renderer>(new graphics::opengl::scene_renderer(*resource_manager_));
                gl::Enable(gl::DEPTH_TEST);
				QOpenGLWidget::initializeGL();
			}

			void OpenGLWidget::resizeGL(int w, int h) {
                projection_matrix_ = float4x4::perspective(deg_to_rad(45.0f),float(width())/float(height()),0.01f,1000.0f);

				gl::Viewport(0,0,width(),height());

				QOpenGLWidget::resizeGL(w,h);
			}

			void OpenGLWidget::paintGL() {
                view_matrix_ = trackball_controller_.matrix();

				gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
                gl::ClearColor(.75,.75,.75,1);

                scene_render_->render(projection_matrix_,view_matrix_,*scene_);

				QOpenGLWidget::paintGL();
			}

			void OpenGLWidget::mousePressEvent(QMouseEvent *e) {
                if(e->button() == Qt::MiddleButton) {
                    trackball_controller_.begin_rotate(convert(e->pos()));
					this->update();
				}
				QOpenGLWidget::mousePressEvent(e);
			}

			void OpenGLWidget::mouseMoveEvent(QMouseEvent *e) {
                trackball_controller_.update(convert(e->pos()));
                this->update();
			}

			void OpenGLWidget::mouseReleaseEvent(QMouseEvent *e) {
                if(e->button() == Qt::MiddleButton) {
                    trackball_controller_.end_rotate(convert(e->pos()));
					this->update();
				}
			}

			void OpenGLWidget::wheelEvent(QWheelEvent *e) {
                trackball_controller_.zoom(e->delta());
				this->update();
				QOpenGLWidget::wheelEvent(e);
			}

			void OpenGLWidget::keyPressEvent(QKeyEvent *e) {
                trackball_controller_.begin_pan();
                QOpenGLWidget::keyPressEvent(e);
            }

            void OpenGLWidget::keyReleaseEvent(QKeyEvent *e) {
                trackball_controller_.end_pan();
                QOpenGLWidget::keyReleaseEvent(e);
            }

            float2 OpenGLWidget::convert(QPoint p) const {
                return float2( remap<float>(p.x(), 0.0f, width() , -1.0f, 1.0f),
                              -remap<float>(p.y(), 0.0f, height(), -1.0f, 1.0f));
            }
		}
	}
}
