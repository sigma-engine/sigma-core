#ifndef SIGMAFIVE_OBJECT_HPP
#define SIGMAFIVE_OBJECT_HPP

#include <sigmafive/config.hpp>

#include <string>
#include <memory>
#include <unordered_map>

#define SIGMAFIVE_CLASS(...) \
public:\
    static const sigmafive::class_uid CLASS_ID;\
    static sigmafive::meta_class &klass;\
private:

#define EXPORT_SIGMAFIVE_CLASS(C) \
    const sigmafive::class_uid C::CLASS_ID = sigmafive::compile_time_hash(#C);\
    sigmafive::meta_class &C::klass = sigmafive::object::add_meta_class<C>(#C,sigmafive::compile_time_hash(#C));

namespace sigmafive {
    class object;
    class meta_class;

    using class_uid = unsigned long long;

    //http://www.cse.yorku.ca/~oz/hash.html
    constexpr const class_uid compile_time_hash(const char *input) {
        return *input ? static_cast<class_uid>(*input) + 33 * compile_time_hash(input + 1) : 5381;
    }

    class SIGMAFIVE_API meta_class {		
    public:
        std::string name() const;

        class_uid uid() const;
    private:
        friend class object;
        meta_class(class_uid uid,std::string name);

        std::string name_;
        class_uid uid_;
    };

    class SIGMAFIVE_API object {
        SIGMAFIVE_CLASS()
    public:
		object() = default;

		object(const object &) = delete;

		object &operator=(const object &) = delete;

        virtual ~object() = default;

        static bool has_meta_class(class_uid uid);

        static const meta_class &meta_class_for(class_uid uid);

        template<class T>
        static bool has_meta_class() {
            return has_meta_class(T::CLASS_ID);
        }

        template<typename T>
        static const meta_class &meta_class_for() {
            return meta_class_for(T::CLASS_ID);
        }
    protected:
        template<class T>
        static meta_class &add_meta_class(std::string klass,class_uid uid) {
            auto it = meta_classes().emplace(std::make_pair(uid,meta_class{uid,klass})).first;
            return it->second;
        }
    private:
        static std::unordered_map<class_uid, meta_class> &meta_classes();
    };
}

#endif //SIGMAFIVE_OBJECT_HPP
