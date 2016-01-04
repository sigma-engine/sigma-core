#ifndef SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP
#define SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>

#include <sigmafive/game/entity.hpp>
#include <sigmafive/game/component.hpp>

#include <memory>
#include <bitset>
#include <vector>
#include <unordered_map>

namespace sigmafive {
    namespace game {
        class component_registry;

        class SIGMAFIVE_API component_manager : public cppbr::meta::object {
        CPPBR_META_CLASS()
        public:
            component_manager(component_registry *registry);

            virtual component_mask get_component_mask(entity e) const;

            virtual component *add_component(cppbr::meta::class_uid component_id, entity e);

            virtual bool has_component(cppbr::meta::class_uid component_id, entity e);

            virtual component *get_component(cppbr::meta::class_uid component_id, entity e);

            virtual void remove_component(cppbr::meta::class_uid component_id, entity e);

            virtual void remove_all_components(entity e);

            template<class T>
            inline T *add_component(entity e) {
                return static_cast<T *>(add_component(T::CLASS_ID, e));
            }

            template<class T>
            inline bool has_component(entity e) {
                return has_component(T::CLASS_ID, e);
            }

            template<class T>
            inline T *get_component(entity e) {
                return static_cast<T *>(get_component(T::CLASS_ID, e));
            }

            template<class T>
            inline void remove_component(entity e) {
                remove_component(T::CLASS_ID, e);
            }

        private:
            component_registry *registry_;
            std::vector<component_mask> component_masks_;
            std::unordered_map<cppbr::meta::class_uid, std::unique_ptr<component_pool>> component_pools_;
        };
    }
}

#endif //SIGMAFIVE_GAME_COMPONENT_MANAGER_HPP
