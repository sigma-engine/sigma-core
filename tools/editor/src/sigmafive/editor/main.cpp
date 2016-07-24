#include <sigmafive/editor/editor.hpp>

#include <sigmafive/editor/MainWindow.hpp>

int main(int argc, char* argv[])
{
    sigmafive::editor::editor app(argc, argv);

    sigmafive::editor::MainWindow m;

    m.show();

    std::cout << __cplusplus << std::endl;

    return app.exec();
}
