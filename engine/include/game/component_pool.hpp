#ifndef SIGMAFIVE_GAME_COMPONENT_POOL_HPP
#define SIGMAFIVE_GAME_COMPONENT_POOL_HPP

#include <game/entity.hpp>
#include <game/component.hpp>

#include <vector>
#include <memory>

namespace sigmafive {
	namespace game {
		class component_pool_base {
		public:
			virtual ~component_pool_base();

			virtual component *add_component(entity::entity_id e) = 0;

			virtual component *get_component(entity::entity_id e) = 0;

			virtual void remove_component(entity::entity_id e) = 0;
		private:
		};

		//TODO replace this with a real pool
		template<typename T>
		class component_pool : public component_pool_base {
		public:
			~component_pool() {
			}

			virtual component *add_component(entity::entity_id e) override {
				if(e.index >= components_.size())
					components_.resize(e.index + 1, nullptr);
				components_[e.index] = std::unique_ptr<T>(new T());
				return static_cast<component*>(components_[e.index]);
			}

			virtual component *get_component(entity::entity_id e) override {
				if(e.index >= components_.size())
					return nullptr;
				return static_cast<component*>(components_[e.index]);
			}

			virtual void remove_component(entity::entity_id e) override {
				components_[e.index] = nullptr;
			}
		private:
			std::vector<std::unique_ptr<T>> components_;
		};
	}
}

#endif //SIGMAFIVE_GAME_COMPONENT_POOL_HPP
