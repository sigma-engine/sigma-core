#ifndef SIGMAFIVE_EDITOR_COMPONENT_SYSTEM_MANAGER_HPP
#define SIGMAFIVE_EDITOR_COMPONENT_SYSTEM_MANAGER_HPP

#include <editor/config.hpp>

#include <QObject>
#include <sigmafive/game/component_system_manager.hpp>

namespace sigmafive {
    namespace editor {
        //TODO this needs to become a qt model at some point
        class EDITOR_API component_system_manager : public QObject, public game::component_system_manager {
        Q_OBJECT
        public:
            component_system_manager(QObject *parent = nullptr);

        private:
        };
    }
}

#endif //SIGMAFIVE_EDITOR_COMPONENT_SYSTEM_MANAGER_HPP