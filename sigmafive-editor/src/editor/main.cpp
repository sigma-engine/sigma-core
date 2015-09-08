#include <editor/application.hpp>
#include <editor/widgets/MainWindow.hpp>

int main(int argc, char *argv[]) {
    sigmafive::editor::application app(argc, argv);

    sigmafive::editor::widgets::MainWindow m;
    m.show();

    return app.exec();
}
