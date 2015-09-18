#ifndef SIGMAFIVE_GAME_COMPONENT_HPP
#define SIGMAFIVE_GAME_COMPONENT_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/object.hpp>

#include <sigmafive/factory.hpp>

#include <sigmafive/game/entity.hpp>

#include <bitset>
#include <vector>

namespace sigmafive {
    namespace game {
        //TODO create a dynamic bitset to avoid this max
        static constexpr const std::uint32_t MAX_NUM_COMPONENTS = 64;
        typedef std::bitset<MAX_NUM_COMPONENTS> component_mask;

        class SIGMAFIVE_API component : public object {
        SIGMAFIVE_CLASS()
        public:
            virtual ~component() = default;
        };

        class SIGMAFIVE_API component_pool : public object {
        SIGMAFIVE_CLASS()
        public:
            virtual ~component_pool() = default;

            virtual component *add_component(entity) = 0;

            virtual component *get_component(entity) = 0;

            virtual void remove_component(entity) = 0;
        private:
        };

        template<typename T>
        class simple_component_pool : public component_pool {
        SIGMAFIVE_CLASS()
        public:
            component *add_component(entity e) override {
                if (e.index >= components_.size())
                    components_.resize(e.index + 1);
                //TODO what should happen when component already exists???
                components_[e.index] = std::unique_ptr<T>(new T());
                return static_cast<component *>(components_[e.index].get());
            }

            component *get_component(entity e) override {
                if (e.index >= components_.size())
                    return nullptr;
                return static_cast<component *>(components_[e.index].get());
            }

            void remove_component(entity e) override {
                if (e.index < components_.size())
                    components_[e.index] = nullptr;
                //TODO should this be and error??
            }

        private:
            std::vector<std::unique_ptr<T>> components_; //TODO this thrashes cache
        };

        namespace detail {
            std::uint32_t create_component_id();
        }
    }
}

#endif //SIGMAFIVE_GAME_COMPONENT_HPP
