#ifndef SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP

#include <sigmafive/engine.hpp>
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
                sigmafive::engine *engine_;
                sigmafive::game::entity_manager entity_manager_;
                sigmafive::game::component_manager component_manager_;
                sigmafive::game::component_system_manager component_system_manager_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
