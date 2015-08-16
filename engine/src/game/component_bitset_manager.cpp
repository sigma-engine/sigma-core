#include <game/component_bitset_manager.hpp>

namespace sigmafive {
    namespace game {
        component_bitset_manager::component_bitset_manager()
            : count(0) {
        }

        bitset component_bitset_manager::bitset_for(class_hash class_id) {
            auto it = bitsets_.find(class_id);
            if(it == bitsets_.end())
                it = bitsets_.insert(std::make_pair(class_id,++count)).first;
            return it->second;
        }
    }
}
