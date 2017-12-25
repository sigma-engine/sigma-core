// TEMPLATE(generated.hpp)
#ifndef SIGMA_TRANSFORM_HPP
#define SIGMA_TRANSFORM_HPP

#include <sigma/config.hpp>
#include <sigma/reflect.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace sigma {

struct R_EXPORT() transform {
    BOOST_HANA_DEFINE_STRUCT(
        transform,
        (glm::vec3, position),
        (glm::quat, rotation),
        (glm::vec3, scale));
    glm::mat4 matrix;

    transform(glm::vec3 position = glm::vec3{ 0 }, glm::quat rotation = glm::quat{}, glm::vec3 scale = glm::vec3{ 1 })
        : position(position)
        , rotation(rotation)
        , scale(scale)
    {
    }

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
