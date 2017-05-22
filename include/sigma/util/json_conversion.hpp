#ifndef SIGMA_ENGINE_JSON_CONVERSION_HPP
#define SIGMA_ENGINE_JSON_CONVERSION_HPP

#include <sigma/config.hpp>

#include <json/json.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <boost/algorithm/string/case_conv.hpp>

namespace sigma {
namespace json {
    static bool from_json(const Json::Value& source, bool& output)
    {
        if (source.isConvertibleTo(Json::booleanValue)) {
            output = source.asBool();
            return true;
        }
        return false;
    }

    static bool from_json(const Json::Value& source, int& output)
    {
        if (source.isConvertibleTo(Json::intValue)) {
            output = source.asInt();
            return true;
        }
        return false;
    }

    static bool from_json(const Json::Value& source, long unsigned int& output)
    {
        if (source.isConvertibleTo(Json::uintValue)) {
            output = source.asUInt64();
            return true;
        }
        return false;
    }

    static bool from_json(const Json::Value& source, float& output)
    {
        if (source.isConvertibleTo(Json::realValue)) {
            output = float(source.asDouble());
            return true;
        }
        return false;
    }

    static bool from_json(const Json::Value& source, glm::vec2& output)
    {
        // TODO support x,y
        return source.isArray() && source.size() == 2 && from_json(source[0], output.x) && from_json(source[1], output.y);
    }

    static bool from_json(const Json::Value& source, glm::vec3& output)
    {
        // TODO support x,y,z
        return source.isArray() && source.size() == 3 && from_json(source[0], output.x) && from_json(source[1], output.y) && from_json(source[2], output.z);
    }

    static bool from_json(const Json::Value& source, glm::vec4& output)
    {
        // TODO support x,y,z,w
        return source.isArray() && source.size() == 4 && from_json(source[0], output.x) && from_json(source[1], output.y) && from_json(source[2], output.z) && from_json(source[3], output.w);
    }

    static bool from_json(const Json::Value& source, glm::quat& output)
    {
        glm::vec3 e;
        if (from_json(source, e)) {
            output = glm::quat{ glm::radians(e) };
            return true;
        }
        // TODO support x,y,z,w
        return false;
    }

    static bool from_json(const Json::Value& source, glm::mat2& output)
    {
        return source.isArray() && source.size() == 2 && from_json(source[0], output[0]) && from_json(source[1], output[1]);
    }

    static bool from_json(const Json::Value& source, glm::mat3& output)
    {
        return source.isArray() && source.size() == 3 && from_json(source[0], output[0]) && from_json(source[1], output[1]) && from_json(source[2], output[2]);
    }

    static bool from_json(const Json::Value& source, glm::mat4& output)
    {
        return source.isArray() && source.size() == 4 && from_json(source[0], output[0]) && from_json(source[1], output[1]) && from_json(source[2], output[2]) && from_json(source[3], output[3]);
    }

    static void to_json(bool source, Json::Value& output)
    {
        output = source;
    }

    static void to_json(int source, Json::Value& output)
    {
        output = source;
    }

    static void to_json(unsigned long int source, Json::Value& output)
    {
        output = (Json::UInt64)source;
    }

    static void to_json(float source, Json::Value& output)
    {
        output = source;
    }

    static void to_json(const glm::vec2& source, Json::Value& output)
    {
        // TODO support x,y
        output[0] = source.x;
        output[1] = source.y;
    }

    static void to_json(const glm::vec3& source, Json::Value& output)
    {
        // TODO support x,y,z
        output[0] = source.x;
        output[1] = source.y;
        output[2] = source.z;
    }

    static void to_json(const glm::vec4& source, Json::Value& output)
    {
        // TODO support x,y,z,w
        output[0] = source.x;
        output[1] = source.y;
        output[2] = source.z;
        output[3] = source.w;
    }

    static void to_json(const glm::quat& source, Json::Value& output)
    {
        to_json(glm::degrees(glm::eulerAngles(source)), output);
    }

    static void to_json(const glm::mat2& source, Json::Value& output)
    {
        to_json(source[0], output[0]);
        to_json(source[1], output[1]);
    }

    static void to_json(const glm::mat3& source, Json::Value& output)
    {
        to_json(source[0], output[0]);
        to_json(source[1], output[1]);
        to_json(source[2], output[2]);
    }

    static void to_json(const glm::mat4& source, Json::Value& output)
    {
        to_json(source[0], output[0]);
        to_json(source[1], output[1]);
        to_json(source[2], output[2]);
        to_json(source[3], output[3]);
    }
}
}

#endif // SIGMA_ENGINE_JSON_CONVERSION_HPP
