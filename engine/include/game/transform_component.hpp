#ifndef SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP
#define SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP

#include <math/vec3.hpp>
#include <math/quaternion.hpp>
#include <game/component.hpp>

namespace sigmafive {
	namespace game {
		class transform_component : public game::component {
			SIGMAFIVE_COMPONENT();
		public:
			float3 position;
			float3 scale;
			quaternionf rotation;

			template <typename Archive>
			void serialize(Archive& ar, const unsigned int version) {
				ar & SIGMAFIVE_SERIALIZE_BASE(component);
				ar & boost::serialization::base_object<game::component>(*this);
				ar & position;
				ar & scale;
				ar & rotation;
			}
		};
	}
}

#endif //SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP
