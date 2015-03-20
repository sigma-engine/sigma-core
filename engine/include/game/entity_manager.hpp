#ifndef SIGMAFIVE_GAME_ENTITY_MANAGER_HPP
#define SIGMAFIVE_GAME_ENTITY_MANAGER_HPP

#include <game/entity.hpp>
#include <game/component.hpp>

#include <vector>
#include <memory>
#include <unordered_map>

namespace sigmafive {
	namespace game {
		class entity_manager {
		public:
			~entity_manager();

			entity create();

			bool is_alive(entity e) const;

			void destroy(entity e);
		private:
			std::vector<entity> entities_;
			std::vector<std::size_t> free_entities_;
		};
	}
}

#endif //SIGMAFIVE_GAME_ENTITY_MANAGER_HPP
