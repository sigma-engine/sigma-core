#ifndef SIGMA_TRANSFORM_HPP
#define SIGMA_TRANSFORM_HPP

#include <sigma/config.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace sigma {

struct SIGMA_API transform {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale{ 1.0f };
    glm::mat4 matrix;

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& position;
        ar& rotation;
        ar& scale;
    }

    glm::mat4 get_matrix() const
    {
        return glm::translate(glm::mat4(1), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1), glm::vec3(scale));
    }
};
}

#endif // SIGMA_TRANSFORM_HPP
