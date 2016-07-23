#include "ui_MainWindow.h"
#include <sigmafive/editor/MainWindow.hpp>

#include <QSettings>

#include <sigmafive/editor/editor_game.hpp>

namespace sigmafive {
namespace editor {
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        centralWidget()->hide();

        opengl_widget* gl_widget = new opengl_widget(this);
        connect(gl_widget, &opengl_widget::initialized, this, &MainWindow::context_ready);

        setTabPosition(Qt::DockWidgetArea::AllDockWidgetAreas, QTabWidget::TabPosition::North);
    }

    MainWindow::~MainWindow()
    {
        QSettings settings("sigma-five", "sigma-five-editor");
        settings.setValue("State", this->saveState(0));
        settings.setValue("Geometry", this->saveGeometry());
        delete ui;
    }

    void MainWindow::context_ready(opengl_widget* gl_widget)
    {
        disconnect(gl_widget, &opengl_widget::initialized, this, &MainWindow::context_ready);

        auto game_view = new editor_game("level", gl_widget, this);
        auto console = new dock_widget("console", this);
        auto graph = new dock_widget("graph", this);
        auto project = new dock_widget("project", this);

        addDockWidget(Qt::LeftDockWidgetArea, game_view);
        addDockWidget(Qt::LeftDockWidgetArea, console);
        addDockWidget(Qt::LeftDockWidgetArea, graph);
        addDockWidget(Qt::RightDockWidgetArea, project);

        splitDockWidget(project, game_view, Qt::Horizontal);
        splitDockWidget(game_view, graph, Qt::Horizontal);
        splitDockWidget(game_view, console, Qt::Vertical);

        QSettings settings("sigma-five", "sigma-five-editor");
        this->restoreState(settings.value("State").toByteArray(), 0);
        this->restoreGeometry(settings.value("Geometry").toByteArray());
    }
}
}
