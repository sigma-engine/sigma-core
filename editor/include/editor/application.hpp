#ifndef SIGMAFIVE_EDITOR_APPLICATION_HPP
#define SIGMAFIVE_EDITOR_APPLICATION_HPP

#include <sigmafive/engine.hpp>

#include <QApplication>

namespace sigmafive {
    namespace editor {
        class application : public QApplication, public engine {
        public:
            application(int &argc, char **argv);
        private:
        };
    }
}

#endif //SIGMAFIVE_EDITOR_APPLICATION_HPP
