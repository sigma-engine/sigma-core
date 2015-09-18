#include <editor/widgets/OpenGLWidget.hpp>

#include <editor/application.hpp>
#include <editor/entity_manager.hpp>
#include <editor/component_manager.hpp>
#include <editor/component_system_manager.hpp>

#include <sigmafive/graphics/context_manager.hpp>

#include <sigmafive/game/static_mesh_component_system.hpp>


namespace sigmafive {
    namespace editor {
        namespace widgets {
            OpenGLWidget::OpenGLWidget(QWidget *parent)
                    : QOpenGLWidget(parent),
                      managerModel_(nullptr),
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

            entity_manager *OpenGLWidget::entityManager() {
                return managerModel_;
            }

            void OpenGLWidget::setEntityManager(entity_manager *model) {
                managerModel_ = model;
                emit entityManagerChanged();
            }

            editor::component_manager *OpenGLWidget::componentManager() const {
                return component_manager_;
            }

            void OpenGLWidget::setComponentManager(editor::component_manager *component_manager) {
                component_manager_ = component_manager;
            }

            editor::component_system_manager *OpenGLWidget::componentSystemManager() const {
                return component_system_manager_;
            }

            void OpenGLWidget::setComponentSystemManager(editor::component_system_manager *component_system_manager) {
                component_system_manager_ = component_system_manager;
            }

            graphics::context_manager *OpenGLWidget::contextManager() const {
                return context_manager_;
            }

            void OpenGLWidget::setContextManager(graphics::context_manager *context_manager) {
                OpenGLWidget::context_manager_ = context_manager;
            }

            void OpenGLWidget::initializeGL() {
                context_ = dynamic_cast<editor::application *>(qApp)->graphics_context_manager()->create_context(CONTEXT_UID);
                QOpenGLWidget::initializeGL();
            }

            void OpenGLWidget::resizeGL(int w, int h) {
                projection_matrix_ = float4x4::perspective(deg_to_rad(45.0f), float(width()) / float(height()), 0.01f, 1000.0f);
                context_->resize(uint2{(unsigned int)w,(unsigned int)h});
                QOpenGLWidget::resizeGL(w, h);
            }

            void OpenGLWidget::paintGL() {
                view_matrix_ = trackball_controller_.matrix();

                if (managerModel_ && component_manager_ && component_system_manager_ && context_manager_) {
                    auto static_mesh_system = component_system_manager_->get_component_system<game::static_mesh_component_system>();

                    context_manager_->make_current(context_.get());
                    static_mesh_system->init(context_manager_);
                    static_mesh_system->process(managerModel_, component_manager_);
                }

                context_->render(projection_matrix_, view_matrix_);

                QOpenGLWidget::paintGL();
            }

            void OpenGLWidget::mousePressEvent(QMouseEvent *e) {
                if (e->button() == Qt::MiddleButton) {
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
                if (e->button() == Qt::MiddleButton) {
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
                return float2(remap<float>(p.x(), 0.0f, width(), -1.0f, 1.0f),
                              -remap<float>(p.y(), 0.0f, height(), -1.0f, 1.0f));
            }
        }
    }
}
