#ifndef SIGMAFIVE_GAME_ENTITY_HPP
#define SIGMAFIVE_GAME_ENTITY_HPP

#include <sigmafive/config.hpp>

#include <cinttypes>

namespace sigmafive {
    namespace game {
        struct SIGMAFIVE_API entity {
            entity();

            entity(std::uint32_t index,std::uint32_t version);

            bool operator==(entity o) const;

            bool operator!=(entity o) const;

            bool is_valid() const;

            std::uint32_t index;
            std::uint32_t version;
        };
    }
}

#endif //SIGMAFIVE_GAME_ENTITY_HPP

