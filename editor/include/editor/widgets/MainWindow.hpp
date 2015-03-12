#ifndef SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP

#include <QMainWindow>

namespace sigmafive {
    namespace editor {
        namespace Ui {
            class MainWindow;
        }

        namespace widgets {
            class MainWindow : public QMainWindow {
            Q_OBJECT
            public:
                explicit MainWindow(QWidget *parent = 0);

                ~MainWindow();

            private:
                Ui::MainWindow *ui;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
