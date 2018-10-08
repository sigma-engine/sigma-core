#include <sigma/util/glm_json.hpp>

namespace glm {
void to_json(nlohmann::json& j, const vec2& v)
{
    j[0] = v.x;
    j[1] = v.y;
}

void from_json(const nlohmann::json& j, vec2& v)
{
    v.x = j[0];
    v.y = j[1];
}

void to_json(nlohmann::json& j, const vec3& v)
{
    j[0] = v.x;
    j[1] = v.y;
    j[2] = v.z;
}

void from_json(const nlohmann::json& j, vec3& v)
{
    v.x = j[0];
    v.y = j[1];
    v.z = j[2];
}

void to_json(nlohmann::json& j, const vec4& v)
{
    j[0] = v.x;
    j[1] = v.y;
    j[2] = v.z;
    j[3] = v.w;
}

void from_json(const nlohmann::json& j, vec4& v)
{
    v.x = j[0];
    v.y = j[1];
    v.z = j[2];
    v.w = j[3];
}
///

void to_json(nlohmann::json& j, const mat2& m)
{
    j[0] = m[0];
    j[1] = m[1];
}

void from_json(const nlohmann::json& j, mat2& m)
{
    m[0] = j[0];
    m[1] = j[1];
}

void to_json(nlohmann::json& j, const mat3& m)
{
    j[0] = m[0];
    j[1] = m[1];
    j[2] = m[2];
}

void from_json(const nlohmann::json& j, mat3& m)
{
    m[0] = j[0];
    m[1] = j[1];
    m[2] = j[2];
}

void to_json(nlohmann::json& j, const mat4& m)
{
    j[0] = m[0];
    j[1] = m[1];
    j[2] = m[2];
    j[3] = m[3];
}

void from_json(const nlohmann::json& j, mat4& m)
{
    m[0] = j[0];
    m[1] = j[1];
    m[2] = j[2];
    m[3] = j[3];
}
}
