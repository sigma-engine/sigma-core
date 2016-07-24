#include <sigmafive/editor/opengl_widget.hpp>

#include <memory>

#include <glm/gtc/type_ptr.hpp>

namespace sigmafive {
namespace editor {
    opengl_widget::opengl_widget(QWidget* parent)
        : QOpenGLWidget(parent)
        , context_(nullptr)
        , viewport_(nullptr)
    {

        QSurfaceFormat format;
        format.setDepthBufferSize(24);
        format.setSamples(8);
        setFormat(format);
    }

    opengl_widget::~opengl_widget() {}

    void opengl_widget::set_viewport(graphics::view_port* viewport)
    {
        viewport_ = viewport;
    }

    graphics::texture_cache& opengl_widget::textures()
    {
        return context_->textures();
    }

    graphics::shader_cache& opengl_widget::shaders() { return context_->shaders(); }

    graphics::material_cache& opengl_widget::materials()
    {
        return context_->materials();
    }

    graphics::static_mesh_cache& opengl_widget::static_meshes()
    {
        return context_->static_meshes();
    }

    void opengl_widget::initializeGL()
    {
        context_ = std::make_unique<opengl::context>();
        emit initialized(this);
        QOpenGLWidget::initializeGL();
    }

    void opengl_widget::resizeGL(int w, int h)
    {
        context_->resize(glm::uvec2{ w, h });

        // TODO remove this here
        if (viewport_)
            viewport_->projection_matrix = glm::perspective(glm::radians(45.0f), float(width()) / float(height()), 0.01f, 10000.0f);
        QOpenGLWidget::resizeGL(w, h);
    }

    void opengl_widget::paintGL()
    {
        if (!viewport_ || !viewport_)
            return;

        context_->render(*viewport_);

        QOpenGLWidget::paintGL();
    }
}
}
