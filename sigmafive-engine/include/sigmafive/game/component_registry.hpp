#ifndef SIGMAFIVE_GAME_COMPONENT_REGISTRY_HPP
#define SIGMAFIVE_GAME_COMPONENT_REGISTRY_HPP

#include <sigmafive/factory.hpp>
#include <sigmafive/game/entity.hpp>
#include <sigmafive/game/component.hpp>

#include <vector>
#include <unordered_map>

namespace sigmafive {
    namespace game {
        class component_pool {
        public:
            virtual ~component_pool() = default;

            virtual component *add_component(entity e) = 0;

            virtual component *get_component(entity e) = 0;

            virtual void remove_component(entity e) = 0;
        private:
        };

        template<typename T>
        class simple_component_pool : public component_pool {
        public:
            component *add_component(entity e) override {
                if(e.index >= components_.size())
                    components_.resize(e.index+1);
                //TODO what should happen when component already exists???
                components_[e.index] = std::unique_ptr<T>(new T());
                return static_cast<component*>(components_[e.index].get());
            }

            component *get_component(entity e) override {
                if(e.index >= components_.size())
                    return nullptr;
                return static_cast<component*>(components_[e.index].get());
            }

            void remove_component(entity e) override {
                if(e.index < components_.size())
                    components_[e.index] = nullptr;
                //TODO should this be and error??
            }
        private:
            std::vector<std::unique_ptr<T>> components_; //TODO this thrashes cache
        };

        class component_registry {
        public:
            void register_component(class_uid uid, std::unique_ptr<factory<component_pool>> pool_factory);

            component_mask mask_for(class_uid uid) const;

            std::unique_ptr<component_pool> create_component_pool_for(class_uid uid) const;

            //TODO implement this
            //void unregister_component(class_uid uid);
        private:
            struct component_register {
                class_uid uid;
                std::unique_ptr<factory<component_pool>> pool_factory;
                component_mask mask;
            };
            std::unordered_map<class_uid,component_register> registered_components_;
        };
    }
}

#endif //SIGMAFIVE_GAME_COMPONENT_REGISTRY_HPP
