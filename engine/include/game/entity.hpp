#ifndef SIGMAFIVE_GAME_ENTITY_HPP
#define SIGMAFIVE_GAME_ENTITY_HPP

#include <cstdint>
#include <functional>

namespace sigmafive {
	namespace game {
		struct entity {
			std::uint32_t index;
			std::uint32_t version;

			bool operator<(entity e) const;

			bool operator==(entity e) const;

			operator std::uint64_t() const;
		};
	}
}

namespace std {
	template<>
	struct hash<sigmafive::game::entity> {
		std::uint64_t operator()(sigmafive::game::entity e) const {
			return (std::uint64_t)e;
		}
	};
}

#endif //SIGMAFIVE_GAME_ENTITY_HPP
