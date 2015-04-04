#ifndef SIGMAFIVE_GAME_ENTITY_HPP
#define SIGMAFIVE_GAME_ENTITY_HPP

#include <cinttypes>

#include <boost/serialization/nvp.hpp>

namespace sigmafive {
	namespace game {
		struct entity {
			std::uint32_t index;
			std::uint32_t version;

			entity();

			entity(std::uint32_t index,std::uint32_t version);

			entity(const entity &) = default;

			entity(entity &&) = default;

			~entity() = default;

			entity &operator=(const entity &) = default;

			entity &operator=(entity &&) = default;

			bool operator==(const entity &o) const;

			bool operator!=(const entity &o) const;

			bool is_valid() const;

			template<class Archive>
			void serialize(Archive &ar, const unsigned int format_version) {
				ar & BOOST_SERIALIZATION_NVP(index);
				ar & BOOST_SERIALIZATION_NVP(version);
			}
		};
	}
}


#endif //SIGMAFIVE_GAME_ENTITY_HPP
