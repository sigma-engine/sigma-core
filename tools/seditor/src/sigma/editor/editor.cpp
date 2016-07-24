#include <boost/filesystem.hpp>
#include <iostream>
#include <sigma/editor/editor.hpp>

namespace sigma {
namespace editor {
    editor::editor(int& argc, char** argv)
        : QApplication(argc, argv)
    {
        inst_ = this;
    }
}
}

sigma::editor::editor* sigma::editor::editor::inst_ = nullptr;
