#ifndef SIGMA_GRAPHICS_OPENGL_LIGHTING_H
#define SIGMA_GRAPHICS_OPENGL_LIGHTING_H

#include <geometry_buffer.glsl>

#define PI 3.1415926535897932384626433832795028841971

struct directional_light {
    vec3 color;
    vec3 direction;
    float intensity;
};

struct point_light {
    vec3 color;
    vec3 position;
    float radius;
    float falloff;
    float intensity;
};

vec3 schlick(vec3 F0, float LdotH)
{
    return F0 + (1 - F0) * (pow(1 - LdotH, 5));
}

float ggx(float alpha, float NdotH)
{
    float alpha2 = alpha * alpha;
    float dnom = NdotH * NdotH * (alpha2 - 1) + 1;
    return alpha2 / (PI * dnom * dnom);
}

float cook_torrance(float NdotH, float NdotV, float VdotH, float NdotL)
{
    float v = min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH);
    return min(v, 1);
}

vec3 compute_lighting(surface s, vec3 L, vec3 V)
{
    vec3 N = s.normal;
    vec3 H = normalize(V + L);
    float LdotH = clamp(dot(L, H), 0, 1);
    float NdotH = clamp(dot(N, H), 0, 1);
    float VdotH = clamp(dot(V, H), 0, 1);
    float NdotL = clamp(dot(N, L), 0, 1);
    float NdotV = clamp(dot(N, V), 0, 1);

    float alpha = s.roughness * s.roughness;

    vec3 albedo = s.diffuse - s.diffuse * s.metalness;
    vec3 realSpecular = mix(vec3(0.03f), s.diffuse, s.metalness);

    vec3 albedoDiffuse = albedo / PI;
    vec3 specular = cook_torrance(NdotH, NdotV, VdotH, NdotL) * ggx(alpha, NdotH) * schlick(realSpecular, VdotH);

    return NdotL * (albedoDiffuse * (1 - specular) + specular);
}

#endif // SIGMA_GRAPHICS_OPENGL_LIGHTING_H
