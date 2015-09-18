#ifndef SIGMAFIVE_EDITOR_COMPONENT_MANAGER_HPP
#define SIGMAFIVE_EDITOR_COMPONENT_MANAGER_HPP

#include <editor/config.hpp>

#include <QObject>
#include <sigmafive/game/component_manager.hpp>

namespace sigmafive {
    namespace editor {
        //TODO this needs to become a qt model at some point
        class EDITOR_API component_manager : public QObject, public game::component_manager {
        Q_OBJECT
        public:
            //TODO replace the game::component_registry with an editor::component_registry
            component_manager(game::component_registry *registry,QObject *parent = nullptr);

        private:
        };
    }
}

#endif //SIGMAFIVE_EDITOR_COMPONENT_MANAGER_HPP
