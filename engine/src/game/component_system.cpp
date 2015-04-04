#include <game/component_system.hpp>

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

SIGMAFIVE_IMPLEMENT_COMPONENT_SYSTEM(sigmafive::game::component_system);