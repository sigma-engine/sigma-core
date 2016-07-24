#ifndef sigma_EDITOR_EDITOR_GAME_HPP
#define sigma_EDITOR_EDITOR_GAME_HPP

#include <sigma/editor/dock_widget.hpp>

#include <QKeyEvent>
#include <QTimer>
#include <QWheelEvent>

#include <sigma/editor/opengl_widget.hpp>
#include <sigma/game.hpp>

namespace sigma {
namespace editor {
    class editor_game : public dock_widget {
        Q_OBJECT
    public:
        static const resource::identifier GRID_FLOOR_MESH;

        editor_game(const QString& level, opengl_widget* gl_widget, QWidget* parent = nullptr);

        void wheelEvent(QWheelEvent* e) override;

        void mousePressEvent(QMouseEvent* e) override;

        void mouseMoveEvent(QMouseEvent* e) override;

        void mouseReleaseEvent(QMouseEvent* e) override;

        void keyPressEvent(QKeyEvent* e) override;

        void keyReleaseEvent(QKeyEvent* e) override;

    private slots:
        void tick();

    private:
        void create_game();

        glm::vec2 convert(QPoint p) const;

        QTimer timer_;
        opengl_widget* gl_widget_;

        trackball_controller trackball_controller_;
        transform_manager transforms_;
        graphics::static_mesh_instance_manager static_mesh_instances_;
        graphics::directional_light_manager directional_lights_;
        graphics::point_light_manager point_lights_;
        graphics::spot_light_manager spot_lights_;

        std::unique_ptr<game> game_;
        std::unique_ptr<graphics::view_port> viewport_;
    };
}
}

#endif //sigma_EDITOR_EDITOR_GAME_HPP
