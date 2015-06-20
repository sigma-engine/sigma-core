#include <context.hpp>

namespace sigmafive {
    void context::set_system(unsigned long class_id, object &system) {
        systems_[class_id] = &system;
    }

    void context::remove_system(object &system) {
        for(auto it =systems_.begin(); it != systems_.end();) {
            if(it->second == &system) {
                systems_.erase(it);
                return;
            }
            ++it;
        }
    }
}
