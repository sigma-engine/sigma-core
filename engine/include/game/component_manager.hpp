#ifndef SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP
#define SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP

#include <game/entity.hpp>
#include <game/component.hpp>
#include <game/component_pool.hpp>
#include <game/component_bitset_manager.hpp>

#include <memory>
#include <vector>
#include <unordered_map>
#include <serialization/unique_ptr.hpp>
#include <boost/serialization/bitset.hpp>
#include <boost/serialization/vector.hpp>

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

            game::component_bitset_manager &bitset_manager();

            bitset get_component_mask(entity e) const;

            component *add_component(unsigned long component_id,entity e);

            bool has_component(unsigned long component_id,entity e);

            component *get_component(unsigned long component_id,entity e);

            void remove_component(unsigned long component_id,entity e);

            void remove_all_components(entity e);

            template<class T>
			T *add_component(entity e) {
                return static_cast<T*>(add_component(T::CLASS_ID,e));
			}

			template<class T>
            bool has_component(entity e) {
                return has_component(T::CLASS_ID,e);
			}

			template<class T>
			T *get_component(entity e) {
                return static_cast<T*>(get_component(T::CLASS_ID,e));
			}

			template<class T>
			void remove_component(entity e) {
                remove_component(T::CLASS_ID,e);
            }

            template<class Archive>
			void serialize(Archive &ar, const unsigned int format_version) {
                //TODO implment this
                //ar & BOOST_SERIALIZATION_NVP(component_masks_);
                //ar & BOOST_SERIALIZATION_NVP(component_pools_);
            }
		private:
			std::vector<bitset> component_masks_;
            game::component_bitset_manager bitset_manager_;
            std::unordered_map<unsigned long,std::unique_ptr<object_pool>> component_pools_;
		};
	}
}

#endif //SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP
