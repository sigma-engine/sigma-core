#ifndef SIGMAFIVE_GAME_ENTITY_HPP
#define SIGMAFIVE_GAME_ENTITY_HPP

#include <cinttypes>

namespace sigmafive {
    namespace game {
        struct entity {
            std::uint32_t index;
            std::uint32_t version;

            entity();

            entity(std::uint32_t index,std::uint32_t version);

            bool operator==(entity o) const;

            bool operator!=(entity o) const;

            bool is_valid() const;
        };
    }
}


#endif //SIGMAFIVE_GAME_ENTITY_HPP

