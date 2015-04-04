#include <game/entity_manager.hpp>

namespace sigmafive {
	namespace game {
		entity entity_manager::create() {
			if(destroyed_entities_.empty()) {
				entities_.push_back(entity{std::uint32_t(entities_.size()),0});
				return entities_.back();
			}
			else {
				auto b = destroyed_entities_.back();
				entities_[b].index = b;
				destroyed_entities_.pop_back();
				return entities_[b];
			}
		}

		bool entity_manager::is_alive(entity e) const {
			return e.index >= 0 && e.index < entities_.size() && entities_[e.index].version == e.version;
		}

		void entity_manager::destroy(entity e) {
			entities_[e.index].version++;
			destroyed_entities_.push_back(e.index);
			entities_[e.index].index = std::uint32_t(-1);
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
