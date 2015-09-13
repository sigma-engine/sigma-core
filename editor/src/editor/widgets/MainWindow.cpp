#include <editor/widgets/MainWindow.hpp>
#include "ui_MainWindow.h"

namespace sigmafive {
    namespace editor {
        namespace widgets {
            MainWindow::MainWindow(entity_manager_model *entityManager, QWidget *parent) :
                    QMainWindow(parent),
                    ui(new Ui::MainWindow),
                    entityManager_(entityManager) {
                ui->setupUi(this);

                ui->openGLWidget->setEntityManager(entityManager_);
                ui->listView->setModel(entityManager_);
            }

            MainWindow::~MainWindow() {
                delete ui;
            }
        }
    }
}
