#ifndef SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP

#include <game/scene.hpp>
#include <system/resource_manager.hpp>
#include <QMainWindow>
#include <game/entity_manager.hpp>

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

            private slots:
                void on_actionImport_triggered();

            private:
                Ui::MainWindow *ui;
                system::resource_manager resource_manager_;
                game::entity_manager entity_manager;
                game::scene scene_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
