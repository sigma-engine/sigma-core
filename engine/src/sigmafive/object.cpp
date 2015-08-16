#include <sigmafive/object.hpp>

namespace sigmafive {
    std::unordered_map<class_hash, meta_class> &object::meta_classes() {
        static std::unordered_map<class_hash, meta_class> meta_classes_;
        return meta_classes_;
    }

    bool object::has_meta_class(class_hash class_id) {
        return meta_classes().count(class_id) != 0;
    }

    const meta_class &object::meta_class_for(class_hash class_id) {
        auto it = meta_classes().find(class_id);
        if(it == meta_classes().end()) {
            throw std::invalid_argument("no meta class for class hash "+std::to_string(class_id));
        }
        return it->second;
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::object)
