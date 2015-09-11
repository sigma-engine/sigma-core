#ifndef SIGMAFIVE_GAME_COMPONENT_HPP
#define SIGMAFIVE_GAME_COMPONENT_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/object.hpp>

#include <bitset>

namespace sigmafive {
    namespace game {
        //TODO create a dynamic bitset to avoid this max
        static constexpr const std::uint32_t MAX_NUM_COMPONENTS = 64;
        typedef std::bitset<MAX_NUM_COMPONENTS> component_mask;

        class SIGMAFIVE_API component : public object {
            SIGMAFIVE_CLASS()
        public:
            virtual ~component() = default;
        };

        namespace detail {
            std::uint32_t create_component_id();
        }
    }
}

#endif //SIGMAFIVE_GAME_COMPONENT_HPP
