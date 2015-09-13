#ifndef SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP

#include <editor/config.hpp>

#include <QMainWindow>

#include <editor/entity_manager_model.hpp>

#include <sigmafive/engine.hpp>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            namespace Ui {
                class MainWindow;
            }

            class EDITOR_API MainWindow : public QMainWindow {
            Q_OBJECT
            public:
                explicit MainWindow(entity_manager_model *entityManager, QWidget *parent = 0);

                ~MainWindow();

            private:
                Ui::MainWindow *ui;
                entity_manager_model *entityManager_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
