#ifndef SIGMAFIVE_GAME_COMPONENT_HPP
#define SIGMAFIVE_GAME_COMPONENT_HPP

#include <sigmafive/object.hpp>

#include <bitset>

namespace sigmafive {
    namespace game {
        //TODO create a dynamic bitset to avoid this max
        static constexpr const std::uint32_t MAX_NUM_COMPONENTS = 64;
        typedef std::bitset<MAX_NUM_COMPONENTS> component_mask;

        class component : public object {
            SIGMAFIVE_CLASS()
        public:
            component() = default;

            component(const component &) = default;

            component(component &&) = default;

            virtual ~component() = default;

            component &operator=(const component &) = default;

            component &operator=(component &&) = default;
        };

        namespace detail {
            std::uint32_t create_component_id();
        }
    }
}

#endif //SIGMAFIVE_GAME_COMPONENT_HPP
