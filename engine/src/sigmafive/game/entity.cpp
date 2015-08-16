#include <game/entity.hpp>

namespace sigmafive {
    namespace game {
        entity::entity()
            : entity(std::uint32_t(-1),std::uint32_t(-1)) {
        }

        entity::entity(std::uint32_t index, std::uint32_t version)
            : index(index), version(version) {
        }

        bool entity::is_valid() const {
            return index != std::uint32_t(-1) && version != std::uint32_t(-1);
        }

        bool entity::operator==(entity e) const {
            return index == e.index && version == e.version;
        }

        bool entity::operator!=(entity e) const {
            return !(this->operator==(e));
        }
    }
}
