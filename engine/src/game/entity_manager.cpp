#include <game/entity_manager.hpp>

namespace sigmafive {
	namespace game {
		entity entity_manager::create() {
			if(free_entities_.empty()) {
				entities_.push_back({std::uint32_t(entities_.size()),std::uint32_t(0)});
				return entities_.back();
			}
			else {
				auto i = free_entities_.back();
				free_entities_.pop_back();
				return entities_[i];
			}
		}

		bool entity_manager::is_alive(entity e) const {
			return e.index >= 0 && e.index < entities_.size() && entities_[e.index].version == e.version;
		}

		void entity_manager::destroy(entity e) {
			if(is_alive(e)) {
				entities_[e.index].version++;
				free_entities_.push_back(e.index);
			}
		}

		entity_manager::~entity_manager() {
			//TODO free components
		}
	}
}
