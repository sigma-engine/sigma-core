#ifndef SIGMA_FIVE_ENTITY_default_component_cache_HPP
#define SIGMA_FIVE_ENTITY_default_component_cache_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/entity/component.hpp>
#include <sigmafive/entity/component_cache.hpp>

#include <vector>
#include <memory>

namespace sigmafive {
    namespace entity {
        template<typename T,class Container=std::vector<T>>
		class default_component_cache : public component_cache<T> {
        public:
            typedef Container container;

			static std::unique_ptr<base_component_cache> factory() {
				return std::make_unique<default_component_cache<T>>();
            }

            default_component_cache(const default_component_cache<T,Container> &) = delete;

            default_component_cache<T,Container> &operator=(const default_component_cache<T,Container> &) = delete;

			void add(entity e) override {
                if(e.is_valid() && e.index >= components.size())
                    components.resize(e.index + 1);
            }

            component<T> get(entity e) override {
                return component<T>{e,this};
            }

            bool remove(entity e) override {
                if(e.is_valid() && e.index < components.size()) // TODO should this check even be here?
                    components[e.index] = T();
            }

            virtual T *acquire(entity e) override {
                return &components[e.index];
            }

            virtual const T *acquire(entity e) const override {
                return &components[e.index];
            }

        private:
            container components;
        };
    }
}

#endif //SIGMA_FIVE_ENTITY_default_component_cache_HPP
