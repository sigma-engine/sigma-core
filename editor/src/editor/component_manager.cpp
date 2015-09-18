#include <editor/component_manager.hpp>

namespace sigmafive {
    namespace editor {
        component_manager::component_manager(game::component_registry *registry, QObject *parent)
                : QObject(parent), game::component_manager(registry) {
        }
    }
}
