#include <game/component.hpp>

namespace sigmafive {
    namespace game {
        namespace detail {
            std::uint32_t create_component_id() {
                static std::uint32_t count = 0;
                return count++;
            }
        }
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::game::component)
