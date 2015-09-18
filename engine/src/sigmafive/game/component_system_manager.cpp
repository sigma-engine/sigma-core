#include <sigmafive/game/component_system_manager.hpp>

#include <sigmafive/game/component_system.hpp>

namespace sigmafive {
    namespace game {
        component_system_manager::iterator component_system_manager::begin() {
            return std::begin(component_systems_);
        }

        component_system_manager::iterator component_system_manager::end() {
            return std::end(component_systems_);
        }

        component_system_manager::const_iterator component_system_manager::begin() const {
            return std::begin(component_systems_);
        }

        component_system_manager::const_iterator component_system_manager::end() const {
            return std::end(component_systems_);
        }


        component_system_manager::const_iterator component_system_manager::cbegin() {
            return component_systems_.cbegin();
        }

        component_system_manager::const_iterator component_system_manager::cend() {
            return component_systems_.cend();
        }
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::game::component_system_manager)