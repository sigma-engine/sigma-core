#ifndef SIGMA_ENGINE_JSON_CONVERSION_HPP
#define SIGMA_ENGINE_JSON_CONVERSION_HPP

#include <json/json-forwards.h>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace sigma {
namespace json {
    bool from_json(const Json::Value& value, float& output);

    bool from_json(const Json::Value& value, glm::vec3& output);

    bool from_json(const Json::Value& value, glm::quat& output);

    Json::Value to_json(glm::vec3 v);
}
}

#endif // SIGMA_ENGINE_JSON_CONVERSION_HPP
