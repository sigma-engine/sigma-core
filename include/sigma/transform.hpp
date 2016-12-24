#ifndef SIGMA_TRANSFORM_HPP
#define SIGMA_TRANSFORM_HPP

#include <sigma/component.hpp>
#include <sigma/config.hpp>
#include <sigma/reflect/reflect.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace sigma {

RCLASS()
struct SIGMA_API transform {

    RPROPERTY()
    glm::vec3 position;

    RPROPERTY()
    glm::vec3 scale{ 1.0f };

    RPROPERTY()
    glm::quat rotation;
};

using transform_manager = basic_component_manager<transform>;
}

#endif // SIGMA_TRANSFORM_HPP
