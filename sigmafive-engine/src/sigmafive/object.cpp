#include <sigmafive/object.hpp>

namespace sigmafive {
    std::string meta_class::name() const {
        return name_;
    }

    class_uid meta_class::uid() const {
        return uid_;
    }

    meta_class::meta_class(class_uid uid, std::string name)
        : uid_(uid),name_(name) {
    }

    bool object::has_meta_class(class_uid uid) {
        return meta_classes().count(uid) > 0;
    }

    std::unordered_map<class_uid, meta_class> &object::meta_classes() {
        static std::unordered_map<class_uid, meta_class> meta_classes_;
        return meta_classes_;
    }

}

EXPORT_SIGMAFIVE_CLASS(sigmafive::object)
