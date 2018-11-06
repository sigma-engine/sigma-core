#ifndef SIGMA_CORE_RESOURCE_HPP
#define SIGMA_CORE_RESOURCE_HPP

#include <sigma/util/filesystem.hpp>

#define CEREAL_FUTURE_EXPERIMENTAL
#include <cereal/archives/adapters.hpp>
#include <cereal/cereal.hpp>

#include <boost/filesystem/path.hpp>

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
    CEREAL_CLASS_VERSION(Klass, Version)

#define resource_name(Klass) sigma::resource::resource_traits<Klass>::fullname
#define resource_shortname(Klass) sigma::resource::resource_traits<Klass>::shortname

namespace sigma {
class context;
namespace resource {
    template <class T>
    struct resource_traits;

    using key_type = boost::filesystem::path;

    template <class T>
    class handle_type {
    public:
        handle_type(std::shared_ptr<T> rsc = nullptr)
            : rsc_(rsc)
        {
        }

        T* get() const noexcept
        {
            return rsc_.get();
        }

        T& operator*() const noexcept
        {
            return *rsc_;
        }

        T* operator->() const noexcept
        {
            return rsc_.get();
        }

        explicit operator bool() const noexcept
        {
            if (rsc_)
                return true;
            return false;
        }

        template <class Archive>
        void save(Archive& ar) const
        {
            ar(rsc_->key());
        }

        template <class Archive>
        void load(Archive& ar)
        {
            auto ctx = cereal::get_user_data<std::shared_ptr<context>>(ar);

            key_type key;
            ar(key);

            *this = ctx->template cache<T>()->get(key);
        }

    private:
        std::shared_ptr<T> rsc_;
    };

    class base_resource {
    public:
        base_resource(std::weak_ptr<sigma::context> context, key_type key);

        base_resource(const base_resource&) = delete;

        base_resource(base_resource&&) = default;

        base_resource& operator=(const base_resource&) = delete;

        base_resource& operator=(base_resource&&) = default;

        virtual ~base_resource() = default;

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
