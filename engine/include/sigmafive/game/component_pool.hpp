#ifndef SIGMAFIVE_GAME_COMPONENT_POOL_HPP
#define SIGMAFIVE_GAME_COMPONENT_POOL_HPP

#include <sigmafive/object.hpp>
#include <sigmafive/game/entity.hpp>

#include <memory>
#include <vector>

namespace sigmafive {
    namespace game {
        class component;
        class component_pool_base : public object_pool {
        public:
            virtual ~component_pool_base() = default;

            virtual component *add_component(entity e) = 0;

            virtual component *get_component(entity e) = 0;

            virtual void remove_component(entity e) = 0;
        };

        template<typename T>
        class component_pool : public component_pool_base {
        public:
            static std::unique_ptr<sigmafive::object_pool> create_pool() {
                return std::unique_ptr<sigmafive::object_pool>(new component_pool<T>());
            }

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
    }
}

#endif //SIGMAFIVE_GAME_COMPONENT_POOL_HPP
