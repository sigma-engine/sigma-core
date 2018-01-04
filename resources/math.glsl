#ifndef SIGMA_MATH_GLSL
#define SIGMA_MATH_GLSL

#define PI 3.1415926535897932
#define PI_2 6.2831853071795864

// TODO Hessian normal form
// TODO http://mathworld.wolfram.com/Point-PlaneDistance.html
// TODO http://mathworld.wolfram.com/HessianNormalForm.html
float plane_point_distance(vec4 plane, vec3 point)
{
    return dot(plane, vec4(point, 1)) / length(plane.xyz);
}

float linstep(float low, float high, float v)
{
    return clamp((v - low) / (high - low), 0.0, 1.0);
}

vec2 hammersley(uint i, uint N)
{
    return vec2(float(i) / float(N), float(bitfieldReverse(i)) * 2.3283064365386963e-10);
}

vec2 equirectangular_mapping(vec3 N)
{
    return vec2(0.5 - atan(N.x, N.z) / PI_2, acos(N.y) / PI);
}

vec4 textureEquirectangular(sampler2D tex, vec3 N)
{
    return texture(tex, equirectangular_mapping(N));
}

vec4 textureEquirectangularLod(sampler2D tex, vec3 N, float lod)
{
    return textureLod(tex, equirectangular_mapping(N), lod);
}

#endif // SIGMA_MATH_GLSL
