#include <sigma/entity.hpp>

#include <boost/functional/hash.hpp>

namespace sigma {
entity::entity() noexcept : entity(std::uint32_t(-1), std::uint32_t(-1)) {}

entity::entity(std::uint32_t index, std::uint32_t version) noexcept
    : index(index),
      version(version) {}

bool entity::is_valid() const noexcept
{
    return index != std::uint32_t(-1) && version != std::uint32_t(-1); // TODO should this be and or?
}

bool entity::operator==(entity e) const noexcept
{
    return index == e.index && version == e.version;
}

bool entity::operator!=(entity e) const noexcept { return !(*this == e); }
}

std::size_t std::hash<sigma::entity>::
operator()(const sigma::entity& e) const
{
    std::size_t seed = 0;
    boost::hash_combine(seed, e.index);
    boost::hash_combine(seed, e.version);
    return seed;
}
