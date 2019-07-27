#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

struct TransformComponent {
	glm::vec3 position = { 0, 0, 0 };
	glm::quat rotation = { 0, 0, 0, 1 };
	glm::vec3 scale = {1, 1, 1};

    glm::mat4 local_matrix() const
    {
        return glm::translate(glm::mat4(1), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1), glm::vec3(scale));
    }
};