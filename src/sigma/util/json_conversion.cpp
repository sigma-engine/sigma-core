#include <sigma/util/json_conversion.hpp>

#include <sigma/graphics/texture.hpp>

#include <json/json.h>

#include <boost/algorithm/string/case_conv.hpp>

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

    bool from_json(const Json::Value& value, glm::vec2& output)
    {
        // TODO support x,y
        return value.isArray() && value.size() == 2 && from_json(value[0], output.x) && from_json(value[1], output.y);
    }

    bool from_json(const Json::Value& value, glm::vec3& output)
    {
        // TODO support x,y,z
        return value.isArray() && value.size() == 3 && from_json(value[0], output.x) && from_json(value[1], output.y) && from_json(value[2], output.z);
    }

    bool from_json(const Json::Value& value, glm::vec4& output)
    {
        // TODO support x,y,z,w
        return value.isArray() && value.size() == 4 && from_json(value[0], output.x) && from_json(value[1], output.y) && from_json(value[2], output.z) && from_json(value[3], output.w);
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

    bool from_json(const Json::Value& value, graphics::texture_filter& output)
    {
        auto str = boost::to_lower_copy(value.asString());
        if (str == "nearest") {
            output = graphics::texture_filter::NEAREST;
            return true;
        } else if (str == "linear") {
            output = graphics::texture_filter::LINEAR;
            return true;
        } else if (str == "none") {
            output = graphics::texture_filter::NONE;
            return true;
        }
        return false;
    }

    bool from_json(const Json::Value& value, graphics::texture_format& output)
    {
        auto str = boost::to_lower_copy(value.asString());
        if (str == "rgb8") {
            output = graphics::texture_format::RGB8;
            return true;
        } else if (str == "rgba8") {
            output = graphics::texture_format::RGBA8;
            return true;
        } else if (str == "rgb32f") {
            output = graphics::texture_format::RGB32F;
            return true;
        }

        return false;
    }

    Json::Value to_json(float v)
    {
        return Json::Value{ v };
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
