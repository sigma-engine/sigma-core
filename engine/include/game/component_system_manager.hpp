#ifndef SIGMAFIVE_GAME_COMPONENT_SYSTEM_MANAGER_HPP
#define SIGMAFIVE_GAME_COMPONENT_SYSTEM_MANAGER_HPP

#include <game/component_system.hpp>
#include <serialization/unique_ptr.hpp>

#include <memory>
#include <vector>

namespace sigmafive {
	namespace game {
		class component_system_manager {
		public:
			template<typename T>
			T *add_component_system() {
				if(T::ID >= component_systems_.size())
					component_systems_.resize(T::ID+1);
				auto component_system = new T();
				component_systems_[T::ID] = std::unique_ptr<T>(component_system);
				return component_system;
			}

			template<typename T>
			bool has_component_system() const {
				if(T::ID >= component_systems_.size())
					return false;
				return component_systems_[T::ID] != nullptr;
			}

			template<typename T>
			T *get_component_system() {
				if(T::ID >= component_systems_.size())
					return nullptr;
				return static_cast<T*>(component_systems_[T::ID].get());
			}

			template<class Archive>
			void serialize(Archive &ar, const unsigned int format_version) {
				ar & BOOST_SERIALIZATION_NVP(component_systems_);
			}
		private:
			std::vector<std::unique_ptr<component_system>> component_systems_;
		};
	}
}

#endif //SIGMAFIVE_GAME_COMPONENT_SYSTEM_MANAGER_HPP
