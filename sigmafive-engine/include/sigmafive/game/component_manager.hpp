#ifndef SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP
#define SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP

#include <sigmafive/game/entity.hpp>
#include <sigmafive/game/component.hpp>
#include <sigmafive/game/component_registry.hpp>

#include <memory>
#include <vector>
#include <unordered_map>

namespace sigmafive {
    namespace game {
        class component_manager {
        public:
            component_manager(component_registry &registry);

            component_mask get_component_mask(entity e) const;

            component *add_component(class_uid component_id,entity e);

            bool has_component(class_uid component_id,entity e);

            component *get_component(class_uid component_id,entity e);

            void remove_component(class_uid component_id,entity e);

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

        private:
            component_registry &registry_;
            std::vector<component_mask> component_masks_;
            std::unordered_map<class_uid,std::unique_ptr<component_pool>> component_pools_;
        };
    }
}

#endif //SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP
