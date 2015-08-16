#ifndef SIGMAFIVE_OBJECT_HPP
#define SIGMAFIVE_OBJECT_HPP

#include <type_info.hpp>
#include <object_pool.hpp>
#include <compile_time_hash.hpp>

#include <memory>
#include <unordered_map>
#include <boost/preprocessor.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/serialization.hpp>

#define SIGMAFIVE_OBJECT_()
#define SIGMAFIVE_OBJECT_FOR_EACH_BODY(r, data, elem) BOOST_PP_CAT(data,elem)()

#define SIGMAFIVE_OBJECT(...) \
public:\
    static const char *CLASS; \
    static const sigmafive::class_hash CLASS_ID;\
    static sigmafive::object_info info; \
 private: /*BOOST_PP_SEQ_FOR_EACH(SIGMAFIVE_OBJECT_FOR_EACH_BODY,SIGMAFIVE_OBJECT_,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))*/

#define SIGMAFIVE_IMPLEMENT_OBJECT(C) \
    const char *C::CLASS = #C; \
    const sigmafive::class_hash C::CLASS_ID = compile_time_hash(#C);\
    sigmafive::object_info C::info = sigmafive::object::add_object_info<C>(#C,compile_time_hash(#C));\
    BOOST_CLASS_EXPORT(C);

#define SIGMAFIVE_IMPLEMENT_OBJECT_NOT_SERIALIZABLE(C) \
    const char *C::CLASS = #C; \
    const sigmafive::class_hash C::CLASS_ID = compile_time_hash(#C);\
    sigmafive::object_info C::info = sigmafive::object::add_object_info<C>(#C,compile_time_hash(#C));

#define SIGMAFIVE_SERIALIZE_BASE(base) \
    boost::serialization::make_nvp("base_object",boost::serialization::base_object<base>(*this));

namespace sigmafive {
    using class_hash = unsigned long;
    struct object_info {
        std::unique_ptr<object_pool>(*create_pool)();
    };

    //http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3911
    template<class,class = void>
    struct has_pool_type : std::false_type {};
    template<class T>
    struct has_pool_type<T, void_t<typename T::pool_type>> : std::true_type {};

    class object {
        SIGMAFIVE_OBJECT()
    public:
        object() = default;

        object(const object &) = default;

        object(object &&) = default;

        object& operator =(const object &) = default;

        object& operator =(object &&) = default;

        virtual ~object() = default;

        static bool has_pool(class_hash class_id) {
            auto it = objects_info().find(class_id);
            return  it != objects_info().end() && it->second.create_pool != nullptr;
        }

        static std::unique_ptr<object_pool> create_pool(class_hash class_id) {
            if(!has_pool(class_id))
                throw std::runtime_error("class "+std::to_string(class_id)+" does not have a object pool.");
            return std::move(objects_info().find(class_id)->second.create_pool());
        }

        template<class Archive>
        void serialize(Archive &ar, const unsigned int format_version) {
        }
    protected:
        template <class T>
        static typename std::enable_if<has_pool_type<T>::value,object_info>::type add_object_info(const char *klass,const class_hash class_id) {
            objects_info().emplace(std::make_pair(class_id,object_info{&T::pool_type::create_pool}));//klass,class_id,
            return objects_info()[class_id];
        }

        template <class T>
        static typename std::enable_if<!has_pool_type<T>::value,object_info>::type add_object_info(const char *klass, const class_hash class_id) {
            objects_info().emplace(std::make_pair(class_id,object_info{nullptr}));//klass,class_id,
            return objects_info()[class_id];
        }

    private:
        static std::unordered_map<class_hash,object_info> &objects_info();
    };
}

#endif //SIGMAFIVE_OBJECT_HPP
