#include <editor/widgets/OpenGLWidget.hpp>

#include <editor/application.hpp>

#include <sigmafive/game/static_mesh_component_system.hpp>


namespace sigmafive {
	namespace editor {
		namespace widgets {
            OpenGLWidget::OpenGLWidget(QWidget *parent)
                : QOpenGLWidget(parent),
                  entity_manager_model_(nullptr),
                  context_(nullptr),
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
				context_ = dynamic_cast<editor::application*>(qApp)->graphics_context_manager().create_context(CONTEXT_UID);
                QOpenGLWidget::initializeGL();
			}

			void OpenGLWidget::resizeGL(int w, int h) {
                projection_matrix_ = float4x4::perspective(deg_to_rad(45.0f),float(width())/float(height()),0.01f,1000.0f);
                QOpenGLWidget::resizeGL(w,h);
			}

			void OpenGLWidget::paintGL() {
                view_matrix_ = trackball_controller_.matrix();

                if(entity_manager_model_) {
                    auto entity_manager = entity_manager_model_->entity_manager();
                    auto component_manager = entity_manager_model_->component_manager();
                    auto component_system_manager = entity_manager_model_->component_system_manager();
                    auto context_manager = entity_manager_model_->context_manager();

                    auto static_mesh_system = component_system_manager->get_component_system<game::static_mesh_component_system>();

                    context_manager->make_current(context_.get());
                    static_mesh_system->init(context_manager);
                    static_mesh_system->process(*entity_manager,*component_manager);
                }

				context_->render(projection_matrix_, view_matrix_);

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
