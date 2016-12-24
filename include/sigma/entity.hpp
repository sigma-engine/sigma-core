#ifndef SIGMA_ENTITY_HPP
#define SIGMA_ENTITY_HPP

#include <sigma/config.hpp>

#include <cinttypes>
#include <functional>

namespace sigma {

struct SIGMA_API entity {
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
struct SIGMA_API hash<sigma::entity> {
    size_t operator()(const sigma::entity& e) const;
};
}

#endif // SIGMA_ENTITY_HPP
