#include <game/entity_manager.hpp>

namespace sigmafive {
	namespace game {
		entity_manager::entity_manager() {

		}

		entity_manager::~entity_manager() {
			//TODO free components
		}

		entity entity_manager::create() {
			if(free_entities_.empty()) {
				entity e{ entity::entity_id{std::uint32_t(entities_.size()), 0} };
				entities_.push_back(e);
				return entities_.back();
			}
			else {
				auto i = free_entities_.back();
				free_entities_.pop_back();
				entities_[i].id_.index = i;
				return entities_[i];
			}
		}

		bool entity_manager::is_alive(entity e) const {
			auto eid = e.id();
			return eid.index >= 0 && eid.index < entities_.size() && entities_[eid.index].id() == eid;
		}

		void entity_manager::destroy(entity e) {
			//TODO remove components
			if(is_alive(e)) {
				auto eid = e.id();
				entities_[eid.index].id_.index = std::uint32_t(-1);
				entities_[eid.index].id_.version++;
				entities_[eid.index].component_mask_ = bitset{0};
				free_entities_.push_back(eid.index);
			}
		}

		entity_manager::iterator entity_manager::begin() {
			return boost::make_filter_iterator<is_entity_alive>(entities_.begin(),entities_.end());
		}

		entity_manager::iterator entity_manager::end() {
			return boost::make_filter_iterator<is_entity_alive>(entities_.end(),entities_.end());
		}

		entity_manager::const_iterator entity_manager::begin() const {
			return boost::make_filter_iterator<is_entity_alive>(entities_.cbegin(),entities_.cend());
		}

		entity_manager::const_iterator entity_manager::end() const {
			return boost::make_filter_iterator<is_entity_alive>(entities_.cend(),entities_.cend());
		}

		entity_manager::reverse_iterator entity_manager::rbegin() {
			return reverse_iterator(end());
		}

		entity_manager::reverse_iterator entity_manager::rend() {
			return reverse_iterator(begin());
		}

		entity_manager::const_reverse_iterator entity_manager::rbegin() const {
			return const_reverse_iterator(end());
		}

		entity_manager::const_reverse_iterator entity_manager::rend() const {
			return const_reverse_iterator(begin());
		}

		entity_manager::const_iterator entity_manager::cbegin() {
			return boost::make_filter_iterator<is_entity_alive>(entities_.cbegin(),entities_.cend());
		}

		entity_manager::const_iterator entity_manager::cend() {
			return boost::make_filter_iterator<is_entity_alive>(entities_.cend(),entities_.cend());
		}

		entity_manager::const_reverse_iterator entity_manager::crbegin() const {
			return const_reverse_iterator(end());
		}

		entity_manager::const_reverse_iterator entity_manager::crend() const {
			return const_reverse_iterator(begin());
		}
	}
}
