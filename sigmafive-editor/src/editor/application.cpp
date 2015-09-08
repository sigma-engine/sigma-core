#include <editor/application.hpp>

namespace sigmafive {
    namespace editor {
        application::application(int &argc, char **argv)
            : QApplication(argc,argv), engine(argc,argv) {
        }
    }
}
