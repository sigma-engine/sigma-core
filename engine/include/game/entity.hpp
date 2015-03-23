#ifndef SIGMAFIVE_GAME_ENTITY_HPP
#define SIGMAFIVE_GAME_ENTITY_HPP

#include <game/component.hpp>

#include <cstdint>
#include <functional>

namespace sigmafive {
	namespace game {
		class entity {
		public:
			struct entity_id {
				std::uint32_t index;
				std::uint32_t version;

				bool operator<(entity_id e) const;

				bool operator==(entity_id e) const;

				operator std::uint64_t() const;
			};

			inline entity(entity_id id) : id_(id), component_mask_(0) { }

			inline entity_id id() const { return id_; }

			inline bitset component_mask() const { return component_mask_; }
		private:
			friend class entity_manager;
			entity_id id_;
			bitset component_mask_;
		};
	}
}

#endif //SIGMAFIVE_GAME_ENTITY_HPP
