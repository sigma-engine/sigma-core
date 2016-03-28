#ifndef SIGMAFIVE_ENGINE_ENTITY_HPP
#define SIGMAFIVE_ENGINE_ENTITY_HPP

#include <sigmafive/config.hpp>

#include <cinttypes>
#include <functional>

namespace sigmafive {
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

namespace std {
template <>
struct SIGMAFIVE_API hash<sigmafive::entity> {
    size_t operator()(const sigmafive::entity& e) const;
};
}

#endif // SIGMAFIVE_ENGINE_ENTITY_HPP
