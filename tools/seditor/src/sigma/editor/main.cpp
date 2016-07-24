#include <sigma/editor/editor.hpp>

#include <sigma/editor/MainWindow.hpp>

int main(int argc, char* argv[])
{
    sigma::editor::editor app(argc, argv);

    sigma::editor::MainWindow m;

    m.show();

    return app.exec();
}
