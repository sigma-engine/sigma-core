#ifndef SIGMA_ENGINE_JSON_CONVERSION_HPP
#define SIGMA_ENGINE_JSON_CONVERSION_HPP

#include <sigma/config.hpp>

#include <json/json.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    enum class texture_filter;
}
namespace json {
    bool SIGMA_API from_json(const Json::Value& value, float& output);

    bool SIGMA_API from_json(const Json::Value& value, glm::vec2& output);

    bool SIGMA_API from_json(const Json::Value& value, glm::vec3& output);

    bool SIGMA_API from_json(const Json::Value& value, glm::vec4& output);

    bool SIGMA_API from_json(const Json::Value& value, glm::quat& output);

    bool SIGMA_API from_json(const Json::Value& value, graphics::texture_filter& output);

    Json::Value SIGMA_API to_json(float v);

    Json::Value SIGMA_API to_json(glm::vec3 v);
}
}

#endif // SIGMA_ENGINE_JSON_CONVERSION_HPP
