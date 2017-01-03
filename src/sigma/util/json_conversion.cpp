#include <sigma/util/json_conversion.hpp>

#include <json/json.h>

namespace sigma {
namespace json {
    bool from_json(const Json::Value& value, float& output)
    {
        if (value.isConvertibleTo(Json::realValue)) {
            output = float(value.asDouble());
            return true;
        }
        return false;
    }

    bool from_json(const Json::Value& value, glm::vec3& output)
    {
        // TODO support x,y,z
        return value.isArray() && value.size() == 3 && from_json(value[0], output.x) && from_json(value[1], output.y) && from_json(value[2], output.z);
    }

    bool from_json(const Json::Value& value, glm::quat& output)
    {
        glm::vec3 e;
        if (from_json(value, e)) {
            output = glm::quat{ glm::radians(e) };
            return true;
        }
        // TODO support x,y,z,w
        return false;
    }

    Json::Value to_json(glm::vec3 v)
    {
        Json::Value out(Json::arrayValue);
        out[0] = v.x;
        out[1] = v.y;
        out[2] = v.z;
        return out;
    }
}
}
