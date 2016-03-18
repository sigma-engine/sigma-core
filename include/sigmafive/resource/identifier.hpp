#ifndef SIGMAFIVE_ENGINE_RESOURCE_IDENTIFIER_HPP
#define SIGMAFIVE_ENGINE_RESOURCE_IDENTIFIER_HPP

#include <sigmafive/config.hpp>

#include <functional>
#include <boost/serialization/access.hpp>
#include <sigmafive/util/compile_time_hash.hpp>

namespace sigmafive {
namespace resource {
    struct identifier {
        identifier()
            : value_(-1)
        {
        }
        identifier(const std::string& name)
            : value_(util::compile_time_hash(name.c_str()))
        {
        }

        identifier(const char* name)
            : value_(util::compile_time_hash(name))
        {
        }

        identifier(const identifier& other)
            : value_(other.value_)
        {
        }

        bool operator==(const identifier& other) const noexcept
        {
            return value_ == other.value_;
        }

        bool operator!=(const identifier& other) const noexcept
        {
            return value_ != other.value_;
        }

        std::size_t value() const noexcept
        {
            return value_;
        }

        explicit operator std::size_t() const noexcept
        {
            return value_;
        }

    private:
        std::size_t value_;

        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& value_;
        }
    };
}
}

namespace std {
template <>
struct hash<sigmafive::resource::identifier> {
    size_t operator()(const sigmafive::resource::identifier& id) const { return static_cast<size_t>(id); }
};
}

#endif //SIGMAFIVE_ENGINE_RESOURCE_IDENTIFIER_HPP
