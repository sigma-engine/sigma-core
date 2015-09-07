
#include <QApplication>
#include <editor/widgets/MainWindow.hpp>

int main(int argc,char *argv[]) {
    QApplication app(argc,argv);
    sigmafive::engine engine(argc,argv);
    sigmafive::editor::widgets::MainWindow m(&engine);
    m.show();
    return app.exec();
}
