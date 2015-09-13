#ifndef SIGMAFIVE_EDITOR_APPLICATION_HPP
#define SIGMAFIVE_EDITOR_APPLICATION_HPP

#include <editor/config.hpp>
#include <sigmafive/engine.hpp>

#include <QApplication>

namespace sigmafive {
    namespace editor {
        class EDITOR_API application : public QApplication, public engine {
        public:
            application(int &argc, char **argv);

        private:
        };
    }
}

#endif //SIGMAFIVE_EDITOR_APPLICATION_HPP
