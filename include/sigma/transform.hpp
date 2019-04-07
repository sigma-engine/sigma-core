#ifndef SIGMA_TRANSFORM_HPP
#define SIGMA_TRANSFORM_HPP

#include <sigma/component.hpp>
#include <sigma/config.hpp>
#include <sigma/reflect.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace sigma {

struct R_EXPORT() transform {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::mat4 matrix;

    transform(glm::vec3 position = glm::vec3{ 0 }, glm::quat rotation = glm::quat{}, glm::vec3 scale = glm::vec3{ 1 })
        : position(position)
        , rotation(rotation)
        , scale(scale)
    {
    }

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(position,
        rotation,
        scale);
    }

    glm::mat4 get_matrix() const
    {
        return glm::translate(glm::mat4(1), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1), glm::vec3(scale));
    }
};

namespace json {
    static bool from_json(std::shared_ptr<context> ctx, const Json::Value& source, transform& output)
    {
        if (source.isMember("position"))
            from_json(ctx, source["position"], output.position);

        if (source.isMember("rotation"))
            from_json(ctx, source["rotation"], output.rotation);
            
        if (source.isMember("scale"))
            from_json(ctx, source["scale"], output.scale);
        return true;
    }   
}
}

REGISTER_COMPONENT(sigma::transform)

#endif // SIGMA_TRANSFORM_HPP
