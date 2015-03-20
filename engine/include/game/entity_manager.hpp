#ifndef SIGMAFIVE_GAME_ENTITY_MANAGER_HPP
#define SIGMAFIVE_GAME_ENTITY_MANAGER_HPP

#include <game/entity.hpp>
#include <game/component.hpp>
#include <game/component_pool.hpp>

#include <vector>
#include <memory>
#include <bitset>

namespace sigmafive {
	namespace game {
		//TODO create a dynamic bitset to avoid this max
		static constexpr const std::uint64_t MAX_NUM_COMPONENTS = 64;
		typedef std::bitset<MAX_NUM_COMPONENTS> bitset;

		class entity_manager {
		public:
			~entity_manager();

			entity create();

			bool is_alive(entity e) const;

			template<class T>
			T *add_component(entity e) {
				assert(is_alive(e) && "entity must be alive to add a component to it.");
				assert(!has_component<T>(e) && "can not add component twice to a entity.");

				if(T::ID >= components_by_type_.size())
					components_by_type_.resize(T::ID + 1);

				if(!components_by_type_[T::ID])
					components_by_type_[T::ID] = std::move(std::unique_ptr<component_pool_base>(new component_pool<T>{}));

				component_masks_[e.index] |= bitset{std::uint64_t(1)<<T::ID};
				return static_cast<T*>(components_by_type_[T::ID]->add_component(e));
			}

			template<class T>
			bool has_component(entity e) const {
				assert(is_alive(e) && "entity must be alive to test if it has a component.");
				return (component_masks_[e.index] & bitset{std::uint64_t(1)<<T::ID}) == bitset{std::uint64_t(1)<<T::ID};
			}

			template<class T>
			T *get_component(entity e) {
				assert(is_alive(e) && "entity must be alive to get a component for it.");
				assert(has_component<T>(e) && "can not get a component for an entity if it does not have it.");
				return static_cast<T*>(components_by_type_[T::ID]->get_component(e));
			}

			template<class T>
			void remove_component(entity e) {
				assert(is_alive(e) && "entity must be alive to remove a component from it.");
				assert(has_component<T>(e) && "can not remove a component from an entity that does not have one.");
				components_by_type_[T::ID]->remove_component(e);
				component_masks_[e.index] &= bitset{std::uint64_t(1)<<T::ID};
			}

			void destroy(entity e);
		private:
			std::vector<entity> entities_;
			std::vector<std::size_t> free_entities_;
			std::vector<bitset> component_masks_;
			std::vector<std::unique_ptr<component_pool_base>> components_by_type_;
		};
	}
}

#endif //SIGMAFIVE_GAME_ENTITY_MANAGER_HPP
