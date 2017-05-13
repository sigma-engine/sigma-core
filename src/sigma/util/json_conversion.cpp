#include <sigma/util/json_conversion.hpp>

#include <sigma/graphics/texture.hpp>

#include <json/json.h>

#include <boost/algorithm/string/case_conv.hpp>

namespace sigma {
namespace json {
    bool from_json(const Json::Value& source, bool& output)
    {
        if (source.isConvertibleTo(Json::booleanValue)) {
            output = float(source.asBool());
            return true;
        }
        return false;
    }

    bool from_json(const Json::Value& source, int& output)
    {
        if (source.isConvertibleTo(Json::intValue)) {
            output = float(source.asInt());
            return true;
        }
        return false;
    }

    bool from_json(const Json::Value& source, float& output)
    {
        if (source.isConvertibleTo(Json::realValue)) {
            output = float(source.asDouble());
            return true;
        }
        return false;
    }

    bool from_json(const Json::Value& source, glm::vec2& output)
    {
        // TODO support x,y
        return source.isArray() && source.size() == 2 && from_json(source[0], output.x) && from_json(source[1], output.y);
    }

    bool from_json(const Json::Value& source, glm::vec3& output)
    {
        // TODO support x,y,z
        return source.isArray() && source.size() == 3 && from_json(source[0], output.x) && from_json(source[1], output.y) && from_json(source[2], output.z);
    }

    bool from_json(const Json::Value& source, glm::vec4& output)
    {
        // TODO support x,y,z,w
        return source.isArray() && source.size() == 4 && from_json(source[0], output.x) && from_json(source[1], output.y) && from_json(source[2], output.z) && from_json(source[3], output.w);
    }

    bool from_json(const Json::Value& source, glm::quat& output)
    {
        glm::vec3 e;
        if (from_json(source, e)) {
            output = glm::quat{ glm::radians(e) };
            return true;
        }
        // TODO support x,y,z,w
        return false;
    }

    void to_json(bool source, Json::Value& output)
    {
        output = source;
    }

    void to_json(int source, Json::Value& output)
    {
        output = source;
    }

    void to_json(float source, Json::Value& output)
    {
        output = source;
    }

    void to_json(const glm::vec2& source, Json::Value& output)
    {
        // TODO support x,y
        output[0] = source.x;
        output[1] = source.y;
    }

    void to_json(const glm::vec3& source, Json::Value& output)
    {
        // TODO support x,y,z
        output[0] = source.x;
        output[1] = source.y;
        output[2] = source.z;
    }

    void to_json(const glm::vec4& source, Json::Value& output)
    {
        // TODO support x,y,z,w
        output[0] = source.x;
        output[1] = source.y;
        output[2] = source.z;
        output[3] = source.w;
    }

    void to_json(const glm::quat& source, Json::Value& output)
    {
        to_json(glm::degrees(glm::eulerAngles(source)), output);
    }
}
}
