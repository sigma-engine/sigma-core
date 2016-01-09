#ifndef SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP
#define SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/entity/component.hpp>

#include <cppbr/math/vec3.hpp>
#include <cppbr/math/quaternion.hpp>

#include <cppbr/meta/object.hpp>

namespace sigmafive {
    namespace game {
        class SIGMAFIVE_API transform_component {
        public:
            float3 position;
            float3 scale;
            quaternionf rotation;

            float4x4 matrix();
        };
    }
}

SIGMA_FIVE_ENTITY_COMPONENT(sigmafive::game::transform_component)

#endif //SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP
