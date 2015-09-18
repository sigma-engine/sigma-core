#include <editor/widgets/MainWindow.hpp>
#include "ui_MainWindow.h"

#include <editor/entity_manager.hpp>
#include <editor/component_manager.hpp>
#include <editor/component_system_manager.hpp>

#include <sigmafive/graphics/context_manager.hpp>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            MainWindow::MainWindow(editor::entity_manager *entityManager,
                                   editor::component_manager *componentManager,
                                   editor::component_system_manager *componentSystemManager,
                                   graphics::context_manager *contextManager,
                                   QWidget *parent) :
                    QMainWindow(parent),
                    ui(new Ui::MainWindow),
                    entityManager_(entityManager),
                    componentManager_(componentManager),
                    componentSystemManager_(componentSystemManager),
                    contextManager_(contextManager) {
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
