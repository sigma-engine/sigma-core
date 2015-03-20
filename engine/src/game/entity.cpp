#include <game/entity.hpp>
#include <stdint-gcc.h>

namespace sigmafive {
	namespace game {
		bool entity::operator<(entity e) const  {
			return (std::uint64_t)*this < (std::uint64_t)e;
		}

		bool entity::operator==(entity e) const {
			return index == e.index && version == e.version;
		}

		entity::operator std::uint64_t() const {
			return std::uint64_t(index) | (std::uint64_t(version) << std::uint64_t(32));
		}
	}
}
