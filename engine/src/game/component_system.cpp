#include <game/component_system.hpp>

namespace sigmafive {
	namespace game {
		namespace detail {
			std::uint32_t create_component_system_id() {
				static std::uint32_t count = 0;
				return count++;
            }
        }

        bool component_system::is_intrested(bitset mask) const {
            return (intrested_mask_ & mask) == intrested_mask_;
        }

	}
}

SIGMAFIVE_IMPLEMENT_OBJECT(sigmafive::game::component_system)
