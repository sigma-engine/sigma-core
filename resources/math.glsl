#ifndef SIGMA_MATH_GLSL
#define SIGMA_MATH_GLSL

#define PI 3.1415926535897932
#define PI_2 6.2831853071795864769252867665590

float linstep(float low, float high, float v)
{
    return clamp((v - low) / (high - low), 0.0, 1.0);
}

vec2 hammersley(uint i, uint N)
{
    return vec2(float(i) / float(N), float(bitfieldReverse(i)) * 2.3283064365386963e-10);
}

vec2 sphere_map(vec3 N)
{
    return vec2(0.5 - atan(N.x, N.z) / PI_2, acos(N.y) / PI);
}

vec4 textureSphere(sampler2D tex, vec3 N)
{
    return texture(tex, sphere_map(N));
}

vec4 textureSphereLod(sampler2D tex, vec3 N, float lod)
{
    return textureLod(tex, sphere_map(N), lod);
}

#endif // SIGMA_MATH_GLSL
