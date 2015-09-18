#ifndef SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP

#include <editor/config.hpp>

#include <QMainWindow>

namespace sigmafive {
    namespace graphics {
        class context_manager;
    }
    namespace editor {

        class entity_manager;
        class component_manager;
        class component_system_manager;

        namespace widgets {
            namespace Ui {
                class MainWindow;
            }

            class EDITOR_API MainWindow : public QMainWindow {
            Q_OBJECT
            public:
                explicit MainWindow(editor::entity_manager *entityManager,
                                    editor::component_manager *componentManager,
                                    editor::component_system_manager *componentSystemManager,
                                    graphics::context_manager *contextManager,
                                    QWidget *parent = 0);

                ~MainWindow();

            private:
                Ui::MainWindow *ui;
                editor::entity_manager *entityManager_;
                editor::component_manager *componentManager_;
                editor::component_system_manager *componentSystemManager_;
                graphics::context_manager *contextManager_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
