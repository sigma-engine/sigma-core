#ifndef SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP
#define SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP

#include <sigmafive/object.hpp>
#include <sigmafive/game/entity_manager.hpp>
#include <sigmafive/game/component_manager.hpp>

namespace sigmafive {
    namespace game {
        class component_system : public object {
            SIGMAFIVE_CLASS()
        public:
            virtual ~component_system() = default;

            virtual void process(entity_manager &em,component_manager &cm) = 0;
        protected:
        };

        namespace detail {
            std::uint32_t create_component_system_id();
        }
    }
}

#endif //SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP
