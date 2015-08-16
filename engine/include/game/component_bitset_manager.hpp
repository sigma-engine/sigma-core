#ifndef SIGMAFIVE_GAME_COMPONENT_BITSET_MANAGER_HPP
#define SIGMAFIVE_GAME_COMPONENT_BITSET_MANAGER_HPP

#include <game/component.hpp>

namespace sigmafive {
	namespace game {
        class component_bitset_manager {
        public:
            component_bitset_manager();

            template<typename T>
            bitset bitset_for() {
                return bitset_for(T::CLASS_ID);
            }

            bitset bitset_for(class_hash class_id);
        private:
            long long count;
            std::unordered_map<class_hash,bitset> bitsets_;
		};
	}
}

#endif //SIGMAFIVE_GAME_COMPONENT_BITSET_MANAGER_HPP
