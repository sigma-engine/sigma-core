#ifndef SIGMAFIVE_ENGINE_TRANSFORM_HPP
#define SIGMAFIVE_ENGINE_TRANSFORM_HPP

#include <sigmafive/config.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <sigmafive/component.hpp>
#include <sigmafive/reflect/reflect.hpp>

namespace sigmafive {

RCLASS()
struct SIGMAFIVE_API transform {

    RPROPERTY()
    glm::vec3 position;

    RPROPERTY()
    glm::vec3 scale{ 1.0f };

    RPROPERTY()
    glm::quat rotation;
};

using transform_manager = basic_component_manager<transform>;
}

SIGMAFIVE_COMPONENT(sigmafive::transform)

#endif // SIGMAFIVE_ENGINE_TRANSFORM_HPP
