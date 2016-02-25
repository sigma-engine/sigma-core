#ifndef SIGMAFIVE_ENTITY_COMPONENT_SYSTEM_HPP
#define SIGMAFIVE_ENTITY_COMPONENT_SYSTEM_HPP

#include <sigmafive/config.hpp>

namespace sigmafive {
    namespace entity {
        class entity_manager;
        class component_manager;

        class component_system {
        public:
            component_system() = default;

            component_system(const component_system &) = delete;

            component_system &operator=(const component_system &) = delete;

            virtual ~component_system() = default;

            virtual void process(entity_manager *em, component_manager *cm) = 0;

        protected:
        };
    }
}

#endif //SIGMAFIVE_ENTITY_COMPONENT_SYSTEM_HPP
