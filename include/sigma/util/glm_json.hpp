#ifndef SIGMA_UTIL_GLM_JSON_HPP
#define SIGMA_UTIL_GLM_JSON_HPP

#include <nlohmann/json.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace glm {
void to_json(nlohmann::json& j, const vec2& v);

void from_json(const nlohmann::json& j, vec2& v);

void to_json(nlohmann::json& j, const vec3& v);

void from_json(const nlohmann::json& j, vec3& v);

void to_json(nlohmann::json& j, const vec4& v);

void from_json(const nlohmann::json& j, vec4& v);
///
void to_json(nlohmann::json& j, const mat2& m);

void from_json(const nlohmann::json& j, mat2& m);

void to_json(nlohmann::json& j, const mat3& m);

void from_json(const nlohmann::json& j, mat3& m);

void to_json(nlohmann::json& j, const mat4& m);

void from_json(const nlohmann::json& j, mat4& m);
}

#endif // SIGMA_UTIL_GLM_JSON_HPP
