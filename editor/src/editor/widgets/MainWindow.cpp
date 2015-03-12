#include <editor/widgets/MainWindow.hpp>
#include "ui_MainWindow.h"

namespace sigmafive {
    namespace editor {
        namespace widgets {
            MainWindow::MainWindow(QWidget *parent) :
                    QMainWindow(parent),
                    ui(new Ui::MainWindow) {
                ui->setupUi(this);
            }

            MainWindow::~MainWindow() {
                delete ui;
            }
        }
    }
}