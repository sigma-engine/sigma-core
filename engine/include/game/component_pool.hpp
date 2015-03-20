#ifndef SIGMAFIVE_GAME_COMPONENT_POOL_HPP
#define SIGMAFIVE_GAME_COMPONENT_POOL_HPP

#include <game/entity.hpp>
#include <game/component.hpp>

#include <vector>

namespace sigmafive {
	namespace game {
		class component_pool_base {
		public:
			virtual ~component_pool_base();

			virtual component *add_component(entity e) = 0;

			virtual component *get_component(entity e) = 0;

			virtual void remove_component(entity e) = 0;
		private:
		};

		template<typename T>
		class component_pool : public component_pool_base {
		public:
			virtual component *add_component(entity e) override {
				if(e.index >= components_.size())
					components_.resize(e.index + 1);
				return static_cast<component*>(components_.data()+e.index);
			}

			virtual component *get_component(entity e) override {
				if(e.index >= components_.size())
					return nullptr;
				return static_cast<component*>(components_.data()+e.index);
			}

			virtual void remove_component(entity e) override {
				//TODO can't really do anything here
			}
		private:
			std::vector<T> components_;
		};
	}
}

#endif //SIGMAFIVE_GAME_COMPONENT_POOL_HPP
