#ifndef SIGMAFIVE_GAME_COMPONENT_REGISTRY_HPP
#define SIGMAFIVE_GAME_COMPONENT_REGISTRY_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>

#include <sigmafive/game/entity.hpp>
#include <sigmafive/game/component.hpp>

#include <vector>
#include <unordered_map>

namespace sigmafive {
    namespace game {
        class SIGMAFIVE_API component_registry : public cppbr::meta::object {
        CPPBR_META_CLASS()
        public:
            void register_component(cppbr::meta::class_uid uid, std::unique_ptr<factory<component_pool>> pool_factory);

            component_mask mask_for(cppbr::meta::class_uid uid) const;

            std::unique_ptr<component_pool> create_component_pool_for(cppbr::meta::class_uid uid) const;

            //TODO implement this
            //void unregister_component(cppbr::meta::class_uid uid);
        private:
            struct component_register {
                cppbr::meta::class_uid uid;
                std::unique_ptr<factory<component_pool>> pool_factory;
                component_mask mask;
            };
            std::unordered_map<cppbr::meta::class_uid, component_register> registered_components_;
        };
    }
}

#endif //SIGMAFIVE_GAME_COMPONENT_REGISTRY_HPP
