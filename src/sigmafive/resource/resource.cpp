#include <sigmafive/resource/resource.hpp>

#include <cppbr/meta/object.hpp>

namespace sigmafive {
    namespace resource {
        identifier::identifier(std::uint64_t id) noexcept
            : value(id) {
        }

        identifier::identifier(const std::string &name) noexcept
            : value(cppbr::meta::compile_time_hash(name.data())) {
        }

        bool identifier::operator==(const identifier &o) const noexcept {
            return value == o.value;
        }

        bool identifier::operator!=(const identifier &o) const noexcept {
            return value != o.value;
        }

        bool identifier::is_valid() const noexcept {
            return value != std::numeric_limits<std::uint64_t>::max();
        }
    }
}
