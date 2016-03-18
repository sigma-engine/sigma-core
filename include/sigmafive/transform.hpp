#ifndef SIGMAFIVE_ENGINE_TRANSFORM_HPP
#define SIGMAFIVE_ENGINE_TRANSFORM_HPP

#include <sigmafive/config.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <sigmafive/component.hpp>

namespace sigmafive {
struct SIGMAFIVE_API transform {
    glm::vec3 position;
    glm::vec3 scale{1.0f};
    glm::quat rotation;
};

using transform_manager = basic_component_manager<transform>;
}

SIGMAFIVE_COMPONENT(sigmafive::transform)

#endif //SIGMAFIVE_ENGINE_TRANSFORM_HPP
