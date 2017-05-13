#ifndef SIGMA_ENGINE_JSON_CONVERSION_HPP
#define SIGMA_ENGINE_JSON_CONVERSION_HPP

#include <sigma/config.hpp>

#include <json/json.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    enum class texture_filter;
    enum class texture_format;
}
namespace json {
    bool SIGMA_API from_json(const Json::Value& source, bool& output);

    bool SIGMA_API from_json(const Json::Value& source, int& output);

    bool SIGMA_API from_json(const Json::Value& source, float& output);

    bool SIGMA_API from_json(const Json::Value& source, glm::vec2& output);

    bool SIGMA_API from_json(const Json::Value& source, glm::vec3& output);

    bool SIGMA_API from_json(const Json::Value& source, glm::vec4& output);

    bool SIGMA_API from_json(const Json::Value& source, glm::quat& output);

    void SIGMA_API to_json(bool source, Json::Value& output);

    void SIGMA_API to_json(int source, Json::Value& output);

    void SIGMA_API to_json(float source, Json::Value& output);

    void SIGMA_API to_json(const glm::vec2& source, Json::Value& output);

    void SIGMA_API to_json(const glm::vec3& source, Json::Value& output);

    void SIGMA_API to_json(const glm::vec4& source, Json::Value& output);

    void SIGMA_API to_json(const glm::quat& source, Json::Value& output);
}
}

#endif // SIGMA_ENGINE_JSON_CONVERSION_HPP
