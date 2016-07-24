#include <sigmafive/editor/editor_game.hpp>

//#include <test_game.hpp>

// TODO is this something that glm has?
template <class T>
T remap(T inValue, T inLow, T inHi, T outLow, T outHi)
{
    return (((inValue - inLow) * (outHi - outLow)) / (inHi - inLow)) + outLow;
}

namespace sigmafive {
namespace editor {
    const resource::identifier editor_game::GRID_FLOOR_MESH{
        "static_mesh://editor/grid_floor:grid_floor"
    };

    editor_game::editor_game(const QString& level, opengl_widget* gl_widget,
        QWidget* parent)
        : dock_widget(level, parent)
        , gl_widget_(gl_widget)
        , static_mesh_instances_(gl_widget->static_meshes())
    {
        setFocusPolicy(Qt::StrongFocus);
        setWidget(gl_widget);

        connect(&timer_, SIGNAL(timeout()), this, SLOT(tick()));
        timer_.start();

        create_game();
    }

    void editor_game::wheelEvent(QWheelEvent* e)
    {
        auto geom = gl_widget_->geometry();
        if (viewport_ && geom.contains(e->pos())) {
            trackball_controller_.zoom(e->delta());
            viewport_->view_matrix = trackball_controller_.matrix();
            gl_widget_->update();
        }
        dock_widget::wheelEvent(e);
    }

    void editor_game::mousePressEvent(QMouseEvent* e)
    {
        auto geom = gl_widget_->geometry();
        if (viewport_ && geom.contains(e->pos())) {
            if (e->button() == Qt::MiddleButton)
                trackball_controller_.begin_rotate(convert(e->pos()));
            viewport_->view_matrix = trackball_controller_.matrix();
            gl_widget_->update();
        }
        dock_widget::mousePressEvent(e);
    }

    void editor_game::mouseMoveEvent(QMouseEvent* e)
    {
        auto geom = gl_widget_->geometry();
        if (viewport_ && geom.contains(e->pos())) {
            trackball_controller_.update(convert(e->pos()));
            viewport_->view_matrix = trackball_controller_.matrix();
            gl_widget_->update();
        }
        dock_widget::mouseMoveEvent(e);
    }

    void editor_game::mouseReleaseEvent(QMouseEvent* e)
    {
        auto geom = gl_widget_->geometry();
        if (viewport_ && geom.contains(e->pos())) {
            if (e->button() == Qt::MiddleButton)
                trackball_controller_.end_rotate(convert(e->pos()));
            viewport_->view_matrix = trackball_controller_.matrix();
            gl_widget_->update();
        }
        dock_widget::mouseReleaseEvent(e);
    }

    void editor_game::keyPressEvent(QKeyEvent* e)
    {
        if ((e->modifiers() & Qt::ShiftModifier) == e->modifiers())
            trackball_controller_.begin_pan();

        if (viewport_)
            viewport_->view_matrix = trackball_controller_.matrix();

        gl_widget_->update();

        dock_widget::keyPressEvent(e);
    }

    void editor_game::keyReleaseEvent(QKeyEvent* e)
    {
        if (viewport_ && ((e->modifiers() | Qt::ShiftModifier) != e->modifiers())) {
            trackball_controller_.end_pan();
            viewport_->view_matrix = trackball_controller_.matrix();
            gl_widget_->update();
        }
        dock_widget::keyReleaseEvent(e);
    }

    void editor_game::tick()
    {
        if (game_) {
            static auto start = std::chrono::high_resolution_clock::now();
            auto end = std::chrono::high_resolution_clock::now();
            auto dt = std::chrono::duration_cast<std::chrono::duration<float> >(end - start);
            start = end;
            game_->update(dt);
            gl_widget_->update();
        }
    }

    void editor_game::create_game()
    {
        /*game_ = std::make_unique<test_game>(gl_widget_->textures(),
            gl_widget_->shaders(),
            gl_widget_->materials(),
            gl_widget_->static_meshes(),
            transforms_,
            directional_lights_,
            point_lights_,
            spot_lights_,
            static_mesh_instances_);

        viewport_ = std::unique_ptr<graphics::view_port>(new graphics::view_port{
            game_->entities,
            transforms_,
            static_mesh_instances_,
            glm::mat4(1.0f),
            trackball_controller_.matrix() });

        gl_widget_->set_viewport(viewport_.get());

        auto grid_floor = game_->entities.create();
        game_->transforms.add(grid_floor);
        game_->static_mesh_instances.add(grid_floor, GRID_FLOOR_MESH);*/
    }

    glm::vec2 editor_game::convert(QPoint p) const
    {
        p = gl_widget_->mapFromParent(p);
        auto w = gl_widget_->width();
        auto h = gl_widget_->height();
        return glm::vec2(remap<float>(p.x(), 0.0f, w, -1.0f, 1.0f),
            -remap<float>(p.y(), 0.0f, h, -1.0f, 1.0f));
        return glm::vec2(0.0f, 0.0f);
    }
}
}
