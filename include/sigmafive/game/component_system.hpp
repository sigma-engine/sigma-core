#ifndef SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP
#define SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>

namespace sigmafive {
    namespace game {
        class entity_manager;
        class component_manager;

        class SIGMAFIVE_API component_system : public cppbr::meta::object {
        CPPBR_META_CLASS()
        public:
            virtual ~component_system() = default;

            virtual void process(entity_manager *em, component_manager *cm) = 0;

        protected:
        };

        namespace detail {
            std::uint32_t create_component_system_id();
        }
    }
}

#endif //SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP
