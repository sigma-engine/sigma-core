#include <game/component_manager.hpp>

namespace sigmafive {
	namespace game {
		bitset component_manager::get_component_mask(entity e) const {
			if(e.index >= component_masks_.size())
				return bitset{};
			return component_masks_[e.index];
		}
	}
}
