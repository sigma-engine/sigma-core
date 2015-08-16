#ifndef SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP
#define SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP

#include <object.hpp>
#include <game/entity.hpp>
#include <game/component.hpp>

namespace sigmafive {
	namespace game {
		class component_system : public object {
            SIGMAFIVE_OBJECT()
		public:
			component_system() = default;

			component_system(const component_system &) = default;

			component_system(component_system &&) = default;

			virtual ~component_system() = default;

			component_system &operator=(const component_system &) = default;

			component_system &operator=(component_system &&) = default;

            virtual bool is_intrested(bitset mask) const;

			virtual void entity_added(entity e) = 0;

			virtual void entity_removed(entity e) = 0;

			virtual void process() = 0;
        protected://TODO
            bitset intrested_mask_;
		};

		namespace detail {
			std::uint32_t create_component_system_id();
		}
	}
}

#endif //SIGMAFIVE_GAME_COMPONENT_SYSTEM_HPP
