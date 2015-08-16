#ifndef SIGMAFIVE_CLASS_HPP
#define SIGMAFIVE_CLASS_HPP

#include <type_info.hpp>
#include <compile_time_hash.hpp>

#include <memory>
#include <unordered_map>

#define SIGMAFIVE_CLASS(...) \
public:\
    static const sigmafive::class_hash CLASS_ID;\
    static sigmafive::meta_class &klass;\
private:

#define EXPORT_SIGMAFIVE_CLASS(C) \
    const sigmafive::class_hash C::CLASS_ID = compile_time_hash(#C);\
    sigmafive::meta_class &C::klass = sigmafive::object::add_meta_class<C>(#C,compile_time_hash(#C));

namespace sigmafive {
    using class_hash = unsigned long;

    class object_pool {
    public:
        virtual ~object_pool() = default;
    };

    struct meta_class {
        const char *name;
        std::unique_ptr<object_pool>(*create_pool)();
    };

    //http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3911
    template<class,class = void>
    struct has_pool_type : std::false_type {};
    template<class T>
    struct has_pool_type<T, void_t<typename T::pool_type>> : std::true_type {};

    class object {
        SIGMAFIVE_CLASS()
    public:
        virtual ~object() = default;

        static bool has_meta_class(class_hash class_id);

        template<class T>
        static bool has_meta_class() {
            return has_meta_class(T::CLASS_ID);
        }

        static const meta_class &meta_class_for(class_hash class_id);

        template<typename T>
        static const meta_class &meta_class_for() {
            return meta_class_for(T::CLASS_ID);
        }

    protected:
        template <class T>
        static typename std::enable_if<has_pool_type<T>::value, meta_class &>::type add_meta_class(const char *klass,class_hash class_id) {
            auto it = meta_classes().emplace(std::make_pair(class_id,meta_class{klass,&T::pool_type::create_pool})).first;
            return it->second;
        }

        template <class T>
        static typename std::enable_if<!has_pool_type<T>::value, meta_class &>::type add_meta_class(const char *klass,class_hash class_id) {
            auto it = meta_classes().emplace(std::make_pair(class_id,meta_class{klass,nullptr})).first;
            return it->second;
        }
    private:
        static std::unordered_map<class_hash, meta_class> &meta_classes();
    };
}

#endif //SIGMAFIVE_CLASS_HPP
