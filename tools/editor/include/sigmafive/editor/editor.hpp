#pragma once

#include <QApplication>

namespace sigmafive {
namespace editor {
    class editor : public QApplication {
    public:
        editor(int& argc, char** argv);

    private:
        static editor* inst_;
    };
}
}
