#include <sigmafive/game/entity_manager.hpp>

namespace sigmafive {
    namespace game {
        entity entity_manager::create() {
            if (free_entities_.empty()) {
                entity e{std::uint32_t(entities_.size()), 0};
                entities_.push_back(e);
                return e;
            }
            else {
                auto index = free_entities_.back();
                free_entities_.pop_back();
                entities_[index].index = index;
                entities_[index].version++;
                return entities_[index];
            }
        }

        bool entity_manager::is_alive(entity e) const {
            if (!e.is_valid() || e.index >= entities_.size())
                return false;
            return entities_[e.index] == e;
        }

        void entity_manager::destroy(entity e) {
            if (!e.is_valid() || e.index >= entities_.size())
                return;
            //only destroy the entity once
            if (entities_[e.index].is_valid()) {
                free_entities_.push_back(entities_[e.index].index);
                entities_[e.index].index = std::uint32_t(-1);
            }
        }

        entity_manager::iterator entity_manager::begin() {
            return boost::make_filter_iterator<is_entity_alive>(entities_.begin(), entities_.end());
        }

        entity_manager::iterator entity_manager::end() {
            return boost::make_filter_iterator<is_entity_alive>(entities_.end(), entities_.end());
        }

        entity_manager::const_iterator entity_manager::begin() const {
            return boost::make_filter_iterator<is_entity_alive>(entities_.cbegin(), entities_.cend());
        }

        entity_manager::const_iterator entity_manager::end() const {
            return boost::make_filter_iterator<is_entity_alive>(entities_.cend(), entities_.cend());
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
            return boost::make_filter_iterator<is_entity_alive>(entities_.cbegin(), entities_.cend());
        }

        entity_manager::const_iterator entity_manager::cend() {
            return boost::make_filter_iterator<is_entity_alive>(entities_.cend(), entities_.cend());
        }

        entity_manager::const_reverse_iterator entity_manager::crbegin() const {
            return const_reverse_iterator(end());
        }

        entity_manager::const_reverse_iterator entity_manager::crend() const {
            return const_reverse_iterator(begin());
        }

        std::size_t entity_manager::size() const {
            return entities_.size() - free_entities_.size();
        }
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::game::entity_manager)