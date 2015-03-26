#ifndef SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP
#define SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP

#include <game/component.hpp>
#include <graphics/static_mesh.hpp>

namespace sigmafive {
	namespace game {
		class static_mesh_component : public game::component {
			SIGMAFIVE_COMPONENT();
		public:
			boost::uuids::uuid static_mesh;
		private:
			friend class boost::serialization::access;
			template <typename Archive>
			void serialize(Archive& ar, const unsigned int version) {
				ar & boost::serialization::base_object<game::component>(*this);
			}
		};
	}
}

#endif //SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_HPP
