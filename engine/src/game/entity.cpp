#include <game/entity.hpp>

namespace sigmafive {
	namespace game {
		entity::entity()
			: entity(std::uint32_t(-1),0) {
		}

		entity::entity(std::uint32_t index, std::uint32_t version)
			: index(index), version(version) {
		}

		bool entity::is_valid() const {
			return index != std::uint32_t(-1);
		}

		bool entity::operator==(const entity &o) const {
			return index == o.index && version == o.version;
		}

		bool entity::operator!=(const entity &o) const {
			return index != o.index || version != o.version;
		}
	}
}
