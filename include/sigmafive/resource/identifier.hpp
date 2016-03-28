#ifndef SIGMAFIVE_ENGINE_RESOURCE_IDENTIFIER_HPP
#define SIGMAFIVE_ENGINE_RESOURCE_IDENTIFIER_HPP

#include <sigmafive/config.hpp>

#include <string>
#include <boost/serialization/access.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <sigmafive/util/compile_time_hash.hpp>

namespace sigmafive {
namespace resource {
    struct SIGMAFIVE_API constexpr_identifier {
        constexpr constexpr_identifier()
            : value_(-1)
        {
        }

        constexpr constexpr_identifier(util::hash_type value)
            : value_(value)
        {
        }

        template <std::size_t N>
        constexpr constexpr_identifier(const char(&name)[N])
            : value_(util::compile_time_hash(name))
        {
        }

        constexpr constexpr_identifier(const constexpr_identifier&) = default;

        constexpr constexpr_identifier(constexpr_identifier&&) noexcept = default;

        constexpr_identifier& operator=(const constexpr_identifier&) = default;

        constexpr_identifier& operator=(constexpr_identifier&&) noexcept = default;

        ~constexpr_identifier() = default;

        bool operator==(const constexpr_identifier& other) const noexcept;

        bool operator!=(const constexpr_identifier& other) const noexcept;

        util::hash_type value() const noexcept;

        explicit operator util::hash_type() const noexcept;

    protected:
        util::hash_type value_;

        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& value_;
        }
    };

    struct SIGMAFIVE_API development_identifier : public constexpr_identifier {
    public:
        development_identifier() = default;

        development_identifier(const char* name);

        development_identifier(std::string type, boost::filesystem::path path, boost::filesystem::path root_directroy = boost::filesystem::current_path());

        development_identifier(std::string type, boost::filesystem::path path, std::string sub_name, boost::filesystem::path root_directroy = boost::filesystem::current_path());

        development_identifier(const std::string& name);

        std::string name() const;

    protected:
        std::string name_;

        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& value_;
            ar& name_;
        }
    };

    SIGMAFIVE_API std::ostream& operator<<(std::ostream& os, const constexpr_identifier& id);

    SIGMAFIVE_API std::ostream& operator<<(std::ostream& os, const development_identifier& id);

    using identifier = development_identifier;
}
}

namespace std {
template <>
struct hash<sigmafive::resource::constexpr_identifier> {
    sigmafive::util::hash_type operator()(const sigmafive::resource::constexpr_identifier& id) const
    {
        return static_cast<sigmafive::util::hash_type>(id);
    }
};

template <>
struct hash<sigmafive::resource::development_identifier> {
    sigmafive::util::hash_type operator()(const sigmafive::resource::development_identifier& id) const
    {
        return static_cast<sigmafive::util::hash_type>(id);
    }
};
}

#endif // SIGMAFIVE_ENGINE_RESOURCE_IDENTIFIER_HPP
