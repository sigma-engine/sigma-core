#ifndef SIGMA_GRAPHICS_OPENGL_LIGHTING_H
#define SIGMA_GRAPHICS_OPENGL_LIGHTING_H

#include <geometry_buffer.glsl>

#define PI 3.1415926535897932
#define NO_DIV_BY_ZERO 1e-5

#ifdef SIGMA_ENGINE_VERTEX_SHADER

// per-instance attributes.
layout(location = 4) in vec4 position_radius;
layout(location = 5) in vec4 color_intensity;

out point_light
{
    vec3 position;
    float radius;
    vec3 color;
    float intensity;
}
out_light;

#elif SIGMA_ENGINE_FRAGMENT_SHADER

in point_light
{
    vec3 position;
    float radius;
    vec3 color;
    float intensity;
}
in_light;

#else
#error "This shader type is not supported"
#endif

struct directional_light {
    vec3 color;
    vec3 direction;
    float intensity;
};

float D_ggx(float NdotH, float alpha2)
{
    float denom = NdotH * NdotH * (alpha2 - 1) + 1;
    return alpha2 / (PI * denom * denom);
}

vec3 F_schlick(float VdotH, vec3 F0)
{
    return F0 + (1 - F0) * pow((1 - VdotH), 5);
}

float G1_schlick(float NdotV, float k)
{
    return NdotV / (NdotV * (1 - k) + k);
}

float G_schlick(float NdotL, float NdotV, float roughness)
{
    float k = roughness + 1;
    k = (k * k)/8;
    return G1_schlick(NdotL, k) * G1_schlick(NdotV, k);
}

vec3 BRDF_specular(vec3 L, vec3 V, vec3 N, vec3 F0, float roughness)
{
    vec3 H = normalize(V + L);
    float NdotV = abs(dot(N, V)) + NO_DIV_BY_ZERO;
    float LdotH = clamp(dot(L, H), 0.0, 1.0);
    float NdotH = clamp(dot(N, H), 0.0, 1.0);
    float NdotL = clamp(dot(N, L), 0.0, 1.0) + NO_DIV_BY_ZERO;

    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    return D_ggx(NdotH, alpha2) * F_schlick(LdotH, F0) * G_schlick(NdotL, NdotV, roughness) / (4 * NdotL * NdotV);
}

vec3 BRDF_diffuse(vec3 albedo)
{
    return albedo / PI;
}

vec3 compute_lighting(surface s, vec3 L, vec3 V)
{
    vec3 N = normalize(s.normal);
    float NdotL = clamp(dot(N, L), 0.0, 1.0);

    vec3 albedo = s.diffuse - s.diffuse * s.metalness;
    vec3 F0 = mix(vec3(0.04f), s.diffuse, s.metalness);

    vec3 specular = BRDF_specular(L, V, N, F0, s.roughness);
    vec3 diffuse = BRDF_diffuse(albedo);

    return NdotL * (specular + diffuse * (1 - specular));
}

#endif // SIGMA_GRAPHICS_OPENGL_LIGHTING_H
