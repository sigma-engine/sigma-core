#ifndef SIGMAFIVE_ENGINE_RESOURCE_IDENTIFIER_HPP
#define SIGMAFIVE_ENGINE_RESOURCE_IDENTIFIER_HPP

#include <sigmafive/config.hpp>

#include <functional>
#include <sigmafive/util/compile_time_hash.hpp>

namespace sigmafive {
namespace resource {
    struct identifier {
        identifier()
            : value(-1)
        {
        }
        identifier(const char* name)
            : value(util::compile_time_hash(name))
        {
        }

        identifier(const identifier& other)
            : value(other.value)
        {
        }

        bool operator==(const identifier& other) const noexcept
        {
            return value == other.value;
        }

        bool operator!=(const identifier& other) const noexcept
        {
            return value != other.value;
        }

        explicit operator std::size_t() const noexcept
        {
            return value;
        }

    private:
        std::size_t value;
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
