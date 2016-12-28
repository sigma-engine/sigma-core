#ifndef SIGMA_GRAPHICS_OPENGL_LIGHTING_H
#define SIGMA_GRAPHICS_OPENGL_LIGHTING_H

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

float attenuation(float r, float f, float d)
{
    float denom = d / r + 1.0;
    float attenuation = 1.0 / (denom * denom);
    float t = (attenuation - f) / (1.0 - f);
    return max(t, 0.0);
}

#define PI 3.1415 // TODO PI
float orenNayarDiffuse(vec3 lightDirection, vec3 viewDirection, vec3 surfaceNormal, float roughness, float albedo)
{

    float LdotV = dot(lightDirection, viewDirection);
    float NdotL = dot(lightDirection, surfaceNormal);
    float NdotV = dot(surfaceNormal, viewDirection);

    float s = LdotV - NdotL * NdotV;
    float t = mix(1.0, max(NdotL, NdotV), step(0.0, s));

    float sigma2 = roughness * roughness;
    float A = 1.0 + sigma2 * (albedo / (sigma2 + 0.13) + 0.5 / (sigma2 + 0.33));
    float B = 0.45 * sigma2 / (sigma2 + 0.09);

    return albedo * max(0.0, NdotL) * (A + B * s / t) / PI;
}

vec3 fresnel(vec3 cspec, vec3 n, vec3 l)
{
    vec3 t = vec3(1 - dot(l, n));
    vec3 t2 = t * t;
    vec3 t4 = t2 * t2;
    vec3 t5 = t * t4;
    return cspec + (vec3(1) - cspec) * t5;
}

#endif // SIGMA_GRAPHICS_OPENGL_LIGHTING_H
