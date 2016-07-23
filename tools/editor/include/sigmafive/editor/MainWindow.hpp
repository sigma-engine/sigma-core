#ifndef SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP

#include <QMainWindow>

#include <sigmafive/editor/opengl_widget.hpp>

namespace sigmafive {
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

#endif //SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
