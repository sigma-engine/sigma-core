#include <editor/widgets/OpenGLWidget.hpp>
#include <QMatrix4x4>
#include <QApplication>
#include <sigmafive/game/static_mesh_component_system.hpp>
namespace sigmafive {
	namespace editor {
		namespace widgets {
            OpenGLWidget::OpenGLWidget(QWidget *parent)
                : context_(nullptr), QOpenGLWidget(parent),
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
                context_manager_ = &dynamic_cast<engine*>(qApp)->graphics_context_manager();
                context_ = std::move(context_manager_->create_context(graphics::opengl::context::CLASS_ID));
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

                if(entity_manager_ && component_manager_ && component_system_manager_) {
                    context_manager_->make_current(context_.get());
                    //TODO move this to somewhere else
                    component_system_manager_->get_component_system<game::static_mesh_component_system>()->process(
                                *entity_manager_,*component_manager_);
                    context_->render(projection_matrix_,view_matrix_);
                }

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
