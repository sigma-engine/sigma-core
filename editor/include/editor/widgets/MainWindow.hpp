#ifndef SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP

#include <editor/config.hpp>

#include <QMainWindow>

#include <editor/component_manager.hpp>
#include <editor/entity_manager.hpp>
#include <editor/component_system_manager.hpp>

#include <sigmafive/engine.hpp>
#include <sigmafive/graphics/context_manager.hpp>
#include <sigmafive/game/component_manager.hpp>
#include <sigmafive/game/component_system_manager.hpp>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            namespace Ui {
                class MainWindow;
            }

            class EDITOR_API MainWindow : public QMainWindow {
            Q_OBJECT
            public:
                explicit MainWindow(entity_manager *entityManager,
                                    editor::component_manager *componentManager,
                                    editor::component_system_manager *componentSystemManager,
                                    sigmafive::graphics::context_manager *contextManager,
                                    QWidget *parent = 0);

                ~MainWindow();

            private:
                Ui::MainWindow *ui;
                entity_manager *entityManager_;
                editor::component_manager *componentManager_;
                editor::component_system_manager *componentSystemManager_;
                sigmafive::graphics::context_manager *contextManager_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
