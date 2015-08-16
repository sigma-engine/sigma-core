#ifndef SIGMAFIVE_CONTEXT_HPP
#define SIGMAFIVE_CONTEXT_HPP

#include <sigmafive/object.hpp>
#include <unordered_map>

namespace sigmafive {
	class context {
    public:
        void set_system(class_hash class_id,object &system);

        template<typename T,typename R>
        void set_system(R &system) {
            set_system(T::CLASS_ID,system);
        }

        void remove_system(object &system);
    private:
        std::unordered_map<class_hash,object *> systems_;
	};
}

#endif //SIGMAFIVE_CONTEXT_HPP
