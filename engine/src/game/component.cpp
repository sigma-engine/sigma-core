#include <game/component.hpp>

namespace sigmafive {
	namespace game {
		namespace detail {
			std::uint64_t create_component_id() {
				static std::uint64_t count = 0;
				return count++;
			}
		}

		component::~component() {

		}
	}
}

SIGMAFIVE_IMPLEMENT_COMPONENT(sigmafive::game::component);
