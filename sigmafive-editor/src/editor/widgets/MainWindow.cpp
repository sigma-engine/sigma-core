#include <editor/widgets/MainWindow.hpp>
#include <editor/widgets/OpenGLWidget.hpp>
#include "ui_MainWindow.h"

#include <sigmafive/graphics/static_mesh.hpp>

#include <QFileDialog>

#include <iostream>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            MainWindow::MainWindow(entity_manager_model *entityManager, assimp_importer *assimpImporter, QWidget *parent) :
                    QMainWindow(parent),
                    ui(new Ui::MainWindow),
                    entityManager_(entityManager),
                    assimpImporter_(assimpImporter) {
                ui->setupUi(this);

                ui->openGLWidget->setEntityManager(entityManager_);
                ui->listView->setModel(entityManager_);
            }

            MainWindow::~MainWindow() {
                delete ui;
            }

            void MainWindow::on_actionImport_triggered() {
                QString file = QUrl::fromLocalFile(QFileDialog::getOpenFileName(nullptr,"Import","/home/aaron/Desktop/sigma-five/resources","3D Models (*.blend *.dae)")).toString();
                assimpImporter_->import_file(entityManager_,file);
                emit entityManager_->layoutChanged();
            }
        }
    }
}
