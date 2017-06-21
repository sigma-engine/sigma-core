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

#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/case_conv.hpp>

namespace sigma {
namespace json {
    template <class>
    struct type_traits;

    template <class T>
    bool from_json(const Json::Value& source, T& output)
    {
        return type_traits<T>::from(source, output);
    }

    template <class T>
    void to_json(const T& source, Json::Value& output)
    {
        type_traits<T>::to(source, output);
    }

    template <>
    struct type_traits<bool> {
        static bool from(const Json::Value& source, bool& output)
        {
            if (source.isConvertibleTo(Json::booleanValue)) {
                output = source.asBool();
                return true;
            }
            return false;
        }

        static void to(bool source, Json::Value& output)
        {
            output = source;
        }
    };

    template <>
    struct type_traits<int> {
        static bool from(const Json::Value& source, int& output)
        {
            if (source.isConvertibleTo(Json::intValue)) {
                output = source.asInt();
                return true;
            }
            return false;
        }

        static void to(int source, Json::Value& output)
        {
            output = source;
        }
    };

    template <>
    struct type_traits<unsigned long int> {
        static bool from(const Json::Value& source, unsigned long int& output)
        {
            if (source.isConvertibleTo(Json::uintValue)) {
                output = source.asUInt64();
                return true;
            }
            return false;
        }

        static void to(unsigned long int source, Json::Value& output)
        {
            output = (Json::UInt64)source;
        }
    };

    template <>
    struct type_traits<float> {
        static bool from(const Json::Value& source, float& output)
        {
            if (source.isConvertibleTo(Json::realValue)) {
                output = float(source.asDouble());
                return true;
            }
            return false;
        }

        static void to(float source, Json::Value& output)
        {
            output = source;
        }
    };

    template <>
    struct type_traits<glm::vec2> {
        static bool from(const Json::Value& source, glm::vec2& output)
        {
            // TODO support x,y
            return source.isArray() && source.size() == 2 && from_json(source[0], output.x) && from_json(source[1], output.y);
        }

        static void to(const glm::vec2& source, Json::Value& output)
        {
            // TODO support x,y
            output[0] = source.x;
            output[1] = source.y;
        }
    };

    template <>
    struct type_traits<glm::vec3> {
        static bool from(const Json::Value& source, glm::vec3& output)
        {
            // TODO support x,y,z
            return source.isArray() && source.size() == 3 && from_json(source[0], output.x) && from_json(source[1], output.y) && from_json(source[2], output.z);
        }

        static void to(const glm::vec3& source, Json::Value& output)
        {
            // TODO support x,y,z
            output[0] = source.x;
            output[1] = source.y;
            output[2] = source.z;
        }
    };

    template <>
    struct type_traits<glm::vec4> {
        static bool from(const Json::Value& source, glm::vec4& output)
        {
            // TODO support x,y,z,w
            return source.isArray() && source.size() == 4 && from_json(source[0], output.x) && from_json(source[1], output.y) && from_json(source[2], output.z) && from_json(source[3], output.w);
        }

        static void to(const glm::vec4& source, Json::Value& output)
        {
            // TODO support x,y,z,w
            output[0] = source.x;
            output[1] = source.y;
            output[2] = source.z;
            output[3] = source.w;
        }
    };

    template <>
    struct type_traits<glm::quat> {
        static bool from(const Json::Value& source, glm::quat& output)
        {
            glm::vec3 e;
            if (from_json(source, e)) {
                output = glm::quat{ glm::radians(e) };
                return true;
            }
            // TODO support x,y,z,w
            return false;
        }

        static void to(const glm::quat& source, Json::Value& output)
        {
            to_json(glm::degrees(glm::eulerAngles(source)), output);
        }
    };

    template <>
    struct type_traits<glm::mat2> {
        static bool from(const Json::Value& source, glm::mat2& output)
        {
            return source.isArray() && source.size() == 2 && from_json(source[0], output[0]) && from_json(source[1], output[1]);
        }

        static void to(const glm::mat2& source, Json::Value& output)
        {
            to_json(source[0], output[0]);
            to_json(source[1], output[1]);
        }
    };

    template <>
    struct type_traits<glm::mat3> {
        static bool from(const Json::Value& source, glm::mat3& output)
        {
            return source.isArray() && source.size() == 3 && from_json(source[0], output[0]) && from_json(source[1], output[1]) && from_json(source[2], output[2]);
        }

        static void to(const glm::mat3& source, Json::Value& output)
        {
            to_json(source[0], output[0]);
            to_json(source[1], output[1]);
            to_json(source[2], output[2]);
        }
    };

    template <>
    struct type_traits<glm::mat4> {
        static bool from(const Json::Value& source, glm::mat4& output)
        {
            return source.isArray() && source.size() == 4 && from_json(source[0], output[0]) && from_json(source[1], output[1]) && from_json(source[2], output[2]) && from_json(source[3], output[3]);
        }

        static void to(const glm::mat4& source, Json::Value& output)
        {
            to_json(source[0], output[0]);
            to_json(source[1], output[1]);
            to_json(source[2], output[2]);
            to_json(source[3], output[3]);
        }
    };

    template <>
    struct type_traits<boost::filesystem::path> {
        static bool from(const Json::Value& source, boost::filesystem::path& output)
        {
            output = source.asString();
            return true;
        }

        static void to(const boost::filesystem::path& source, Json::Value& output)
        {
            output = source.string();
        }
    };

    template <class T, long int N>
    struct type_traits<T[N]> {
        static bool from(const Json::Value& source, T* output)
        {
            for (long int i = 0; i < N; ++i)
                from_json(source[(int)i], output[i]);
        }

        static void to(const T* source, Json::Value& output)
        {
            for (long int i = 0; i < N; ++i)
                to_json(source[i], output[(int)i]);
        }
    };
}
}

#endif // SIGMA_ENGINE_JSON_CONVERSION_HPP
