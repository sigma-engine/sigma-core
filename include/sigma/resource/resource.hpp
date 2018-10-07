#ifndef SIGMA_CORE_RESOURCE_HPP
#define SIGMA_CORE_RESOURCE_HPP

#include <boost/filesystem/path.hpp>
#include <boost/serialization/serialization.hpp>

#define REGISTER_RESOURCE(Klass, ShortName, Version)             \
    namespace sigma {                                            \
    namespace resource {                                         \
        template <>                                              \
        struct resource_traits<Klass> {                          \
            static constexpr const char* fullname = #Klass;      \
            static constexpr const char* shortname = #ShortName; \
        };                                                       \
    }                                                            \
    }                                                            \
    BOOST_CLASS_VERSION(Klass, Version)

#define resource_name(Klass) sigma::resource::resource_traits<Klass>::fullname
#define resource_shortname(Klass) sigma::resource::resource_traits<Klass>::shortname

namespace sigma {
class context;
namespace resource {
    template <class T>
    struct resource_traits;

    using key_type = boost::filesystem::path;

    class base_resource {
    public:
        base_resource(std::weak_ptr<sigma::context> context, const key_type& key);

        base_resource(const base_resource&) = delete;

        base_resource(base_resource&&) = default;

        base_resource& operator=(const base_resource&) = delete;

        base_resource& operator=(base_resource&&) = default;

        virtual ~base_resource();

        std::weak_ptr<sigma::context> context();

        const key_type& key() const;

        size_t id() const;

        void set_id(size_t id);

    private:
        std::weak_ptr<sigma::context> context_;
        key_type key_;
        size_t id_;
    };
}
}

#endif // SIGMA_CORE_RESOURCE_HPP
