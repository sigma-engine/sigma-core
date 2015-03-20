#include <game/component.hpp>

namespace sigmafive {
	namespace game {
		namespace detail {
			unsigned int create_component_id() {
				static unsigned int count;
				return count++;
			}
		}

		component::~component() {

		}
	}
}

SIGMAFIVE_IMPLEMENT_COMPONENT(sigmafive::game::component);
