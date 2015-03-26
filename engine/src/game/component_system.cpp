#include <game/component_system.hpp>

namespace sigmafive {
	namespace game {
		namespace detail {
			std::uint64_t create_component_system_id() {
				static std::uint64_t count = 0;
				return count++;
			}
		}

		component_system::~component_system() {
		}
	}
}

SIGMAFIVE_IMPLEMENT_COMPONENT_SYSTEM(sigmafive::game::component_system);