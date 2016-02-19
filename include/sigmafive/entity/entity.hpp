#ifndef SIGMAFIVE_ENTITY_ENTITY_HPP
#define SIGMAFIVE_ENTITY_ENTITY_HPP

#include <sigmafive/config.hpp>

#include <cinttypes>

namespace sigmafive {
    namespace entity {
        struct SIGMAFIVE_API entity {
            entity() noexcept;

            entity(std::uint32_t index, std::uint32_t version) noexcept;

            bool operator==(entity o) const noexcept;

            bool operator!=(entity o) const noexcept;

            bool is_valid() const noexcept;

            std::uint32_t index;
            std::uint32_t version;
        };
    }
}

#endif //SIGMAFIVE_ENTITY_ENTITY_HPP

