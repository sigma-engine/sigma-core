#ifndef SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP
#define SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/object.hpp>

#include <sigmafive/math/vec3.hpp>
#include <sigmafive/math/quaternion.hpp>

#include <sigmafive/game/component.hpp>
#include <sigmafive/game/component_registry.hpp>

namespace sigmafive {
    namespace game {
        class SIGMAFIVE_API transform_component : public game::component {
        SIGMAFIVE_CLASS()
        public:
            float3 position;
            float3 scale;
            quaternionf rotation;

            float4x4 matrix();
        };

        using transform_component_pool = simple_component_pool<transform_component>;
        using transform_component_pool_factory = simple_factory<component_pool, transform_component_pool>;
    }
}

#endif //SIGMAFIVE_GAME_TRANSFORM_COMPONENT_HPP
