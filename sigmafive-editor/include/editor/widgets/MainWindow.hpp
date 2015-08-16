#ifndef SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP

#include <sigmafive/game/scene.hpp>
#include <sigmafive/game/entity_world.hpp>
#include <sigmafive/game/component_manager.hpp>
#include <sigmafive/system/resource_manager.hpp>
#include <sigmafive/game/component_system_manager.hpp>
#include <sigmafive/game/static_mesh_component_system.hpp>

#include <QMainWindow>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            namespace Ui {
                class MainWindow;
            }

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
                game::scene scene_;
                game::entity_world world_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
