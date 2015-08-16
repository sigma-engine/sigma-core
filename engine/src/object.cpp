#include <object.hpp>

namespace sigmafive {
    std::unordered_map<class_hash,object_info> &object::objects_info() {
        static std::unordered_map<class_hash,object_info> object_info_;
        return object_info_;
    }
}

SIGMAFIVE_IMPLEMENT_OBJECT(sigmafive::object)
