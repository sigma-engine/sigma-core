#version 400

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>
// clang-format on

const vec3 gamma_correction = vec3(1.0 / 2.2);

void main()
{
    out_image = pow(texture(in_image, in_vertex.texcoord).rgb, gamma_correction);
}
