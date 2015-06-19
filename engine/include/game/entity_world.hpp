#ifndef SIGMAFIVE_GAME_ENTITY_WORLD_HPP
#define SIGMAFIVE_GAME_ENTITY_WORLD_HPP

#include <game/entity_manager.hpp>
#include <game/component_manager.hpp>
#include <game/component_system_manager.hpp>

namespace sigmafive {
	namespace game {
		class entity_world {
		public:
            component_bitset_manager &bitset_manager();

			game::entity create();

			bool is_alive(game::entity e) const;

            template<class T>
            T *add_component_system() {
                //TODO inform the system of entities that aready exist
                return component_system_manager_.add_component_system<T>();
            }

            template<class T>
            bool has_component_system() {
                return component_system_manager_.has_component_system<T>();
            }

            template<class T>
            T *get_component_system() {
                return component_system_manager_.get_component_system<T>();
            }

            template<class T>
            void remove_component_system() {
                //TODO remove entities from the system
                component_system_manager_.remove_component_system<T>();
            }

            template<class T>
			T *add_component(game::entity e) {
                auto premask = component_manager_.get_component_mask(e);
                auto cmp = component_manager_.add_component<T>(e);
                auto postmask = component_manager_.get_component_mask(e);
                for(auto &item:component_system_manager_) {
                    component_system *system = item.second.get();
                    if(!system->is_intrested(premask) && system->is_intrested(postmask)) {
                        system->entity_added(e);
                    }
                }
                return cmp;
			}

			template<class T>
			bool has_component(game::entity e) {
				return component_manager_.has_component<T>(e);
			}

			template<class T>
			T *get_component(game::entity e) {
				return component_manager_.get_component<T>(e);
			}

			template<class T>
			void remove_component(game::entity e) {
                auto premask = component_manager_.get_component_mask(e);
                component_manager_.remove_component<T>(e);
                auto postmask = component_manager_.get_component_mask(e);
                for(auto &item:component_system_manager_) {
                    component_system *system = item.second.get();
                    if(system->is_intrested(premask) && !system->is_intrested(postmask))
                        system->entity_removed(e);
                }
            }

			void destroy(game::entity e);
		private:
			game::entity_manager entity_manager_;
			game::component_manager component_manager_;
			game::component_system_manager component_system_manager_;
		};
	}
}

#endif //SIGMAFIVE_GAME_ENTITY_WORLD_HPP
