#ifndef SIGMAFIVE_GAME_COMPONENT_SYSTEM_MANAGER_HPP
#define SIGMAFIVE_GAME_COMPONENT_SYSTEM_MANAGER_HPP

#include <game/component_system.hpp>
#include <serialization/unique_ptr.hpp>

#include <memory>
#include <vector>

namespace sigmafive {
	namespace game {
		class component_system_manager {
            struct is_entity_alive {
                inline bool operator()(entity e) { return e.index != std::uint32_t(-1); }
            };
		public:
            typedef std::unordered_map<class_hash,std::unique_ptr<component_system>> container;
            typedef container::reference                      						  reference;
            typedef container::const_reference                						  const_reference;
            typedef container::iterator                                               iterator;
            typedef container::const_iterator                                         const_iterator;
            typedef container::size_type                         					  size_type;
            typedef container::difference_type                          			  difference_type;
            typedef container::value_type                               			  value_type;
            typedef container::allocator_type                           			  allocator_type;
            typedef container::pointer       										  pointer;
            typedef container::const_pointer 										  const_pointer;
            typedef std::reverse_iterator<iterator>                     			  reverse_iterator;
            typedef std::reverse_iterator<const_iterator>               			  const_reverse_iterator;

            iterator begin();

            iterator end();

            const_iterator begin() const;

            const_iterator end() const;

            const_iterator cbegin();

            const_iterator cend();

			template<typename T>
			T *add_component_system() {
				auto component_system = new T();
                component_systems_[T::CLASS_ID] = std::unique_ptr<T>(component_system);
				return component_system;
			}

			template<typename T>
			bool has_component_system() const {
                return component_systems_.find(T::CLASS_ID)!= component_systems_.end();
			}

			template<typename T>
			T *get_component_system() {
                auto it = component_systems_.find(T::CLASS_ID);
                if(it == component_systems_.end())
                    return nullptr;
                return static_cast<T*>(it->second.get());
			}

            template<typename T>
            void remove_component_system() {
                auto it = component_systems_.find(T::CLASS_ID);
                if(it != component_systems_.end())
                    component_systems_.erase(it);
            }

			template<class Archive>
			void serialize(Archive &ar, const unsigned int format_version) {
                //TODO implement this
                //ar & BOOST_SERIALIZATION_NVP(component_systems_);
			}
		private:
            container component_systems_;
		};
	}
}

#endif //SIGMAFIVE_GAME_COMPONENT_SYSTEM_MANAGER_HPP
