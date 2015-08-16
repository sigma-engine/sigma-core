#ifndef SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP
#define SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP

#include <math/vec3.hpp>
#include <game/component.hpp>
#include <math/quaternion.hpp>

namespace sigmafive {
	namespace game {
		class transform_component : public game::component {
            SIGMAFIVE_OBJECT()
		public:
            typedef sigmafive::game::component_pool<transform_component> pool_type;
            float3 position;
			float3 scale;
            quaternionf rotation;

            float4x4 matrix() {
                return float4x4::translation(position)  * rotation * float4x4::scale(scale);
            }
		};
	}
}

#endif //SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP
