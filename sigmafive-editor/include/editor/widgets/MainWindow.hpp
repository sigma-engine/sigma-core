#ifndef SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP

#include <QMainWindow>

#include <editor/assimp_importer.hpp>
#include <editor/entity_manager_model.hpp>

#include <sigmafive/engine.hpp>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            namespace Ui {
                class MainWindow;
            }

            class MainWindow : public QMainWindow {
            Q_OBJECT
            public:
                explicit MainWindow(entity_manager_model *entityManager,assimp_importer *assimpImporter,QWidget *parent = 0);

                ~MainWindow();

            private slots:
                void on_actionImport_triggered();

            private:
                Ui::MainWindow *ui;
                entity_manager_model *entityManager_;
                assimp_importer *assimpImporter_;
            };
        }
    }
}

#endif //SIGMAFIVE_EDITOR_WIDGETS_MAINWINDOW_HPP
