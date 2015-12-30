#include <sigmafive/game/transform_component.hpp>

namespace sigmafive {
    namespace game {
        /* float4x4 transform_component::matrix() const {
             return float4x4::transform(scale,rotation,position);
         }*/
        float4x4 transform_component::matrix() {
            return float4x4::translation(position) * rotation * float4x4::scale(scale);
        }
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::game::transform_component)
