#ifndef sigma_EDITOR_WIDGETS_MAINWINDOW_HPP
#define sigma_EDITOR_WIDGETS_MAINWINDOW_HPP

#include <QMainWindow>

#include <sigma/editor/opengl_widget.hpp>

namespace sigma {
namespace editor {

    namespace Ui {
        class MainWindow;
    }

    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget* parent = 0);

        ~MainWindow();

        void context_ready(opengl_widget* gl_widget);

    private:
        Ui::MainWindow* ui;
    };
}
}

#endif //sigma_EDITOR_WIDGETS_MAINWINDOW_HPP
