#include <sigmafive/game/component_system.hpp>

namespace sigmafive {
    namespace game {
        namespace detail {
            std::uint32_t create_component_system_id() {
                static std::uint32_t count = 0;
                return count++;
            }
        }
    }
}

EXPORT_CPPBR_META_CLASS(sigmafive::game::component_system)
