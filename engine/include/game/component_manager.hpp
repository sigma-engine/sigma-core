#ifndef SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP
#define SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP

#include <game/entity.hpp>
#include <game/component.hpp>

#include <memory>
#include <vector>
#include <serialization/unique_ptr.hpp>
#include <boost/serialization/bitset.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/integer_fwd.hpp>

namespace sigmafive {
	namespace game {
		class component_manager {
		public:
			component_manager() = default;

			component_manager(const component_manager &) = delete;

			component_manager(component_manager &&) = default;

			virtual ~component_manager() = default;

			component_manager &operator=(const component_manager &) = delete;

			component_manager &operator=(component_manager &&) = default;

			template<class T>
			T *add_component(entity e) {
				if(T::ID >= component_pools_.size())
					component_pools_.resize(T::ID+1);
				if(e.index >= component_pools_[T::ID].size())
					component_pools_[T::ID].resize(e.index+1);
				if(e.index >= component_masks_.size())
					component_masks_.resize(e.index + 1);
				component_pools_[T::ID][e.index] = std::unique_ptr<component>(new T());
				component_masks_[e.index].set(T::ID,true);
				return static_cast<T*>(component_pools_[T::ID][e.index].get());
			}

			template<class T>
			bool has_component(entity e) const {
				if(e.index >= component_masks_.size())
					return false;
				return component_masks_[e.index].test(T::ID);
			}

			template<class T>
			T *get_component(entity e) {
				if(T::ID >= component_pools_.size() || e.index >= component_pools_[T::ID].size())
					return nullptr;
				return static_cast<T*>(component_pools_[T::ID][e.index].get());
			}

			bitset get_component_mask(entity e) const;

			template<class Archive>
			void serialize(Archive &ar, const unsigned int format_version) {
				ar & BOOST_SERIALIZATION_NVP(component_masks_);
				ar & BOOST_SERIALIZATION_NVP(component_pools_);
			}
		private:
			typedef std::vector<std::unique_ptr<component>> component_pool; //TODO make these real component pools
			std::vector<bitset> component_masks_;
			std::vector<component_pool> component_pools_;
		};
	}
}

#endif //SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP
