#version 460

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>
// clang-format on

vec3 reinhard(vec3 color)
{
    return color / (color + vec3(1.0));
}

const vec3 gamma_correction = vec3(1.0 / 2.2);

void main()
{
    vec3 color = texture(in_image, in_vertex.texcoord).rgb;
    out_image = pow(reinhard(color), gamma_correction);
}
