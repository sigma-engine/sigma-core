#include <boost/filesystem.hpp>
#include <iostream>
#include <sigmafive/editor/editor.hpp>

namespace sigmafive {
namespace editor {
    editor::editor(int& argc, char** argv)
        : QApplication(argc, argv)
    {
        inst_ = this;
    }
}
}

sigmafive::editor::editor* sigmafive::editor::editor::inst_ = nullptr;
