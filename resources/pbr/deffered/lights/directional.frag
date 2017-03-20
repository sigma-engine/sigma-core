#version 400

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <pbr/brdf.glsl>
// clang-format on

in directional_light
{
    vec3 color;
    float intensity;
    vec3 direction;
} in_light;

void main()
{
    surface s = read_geometry_buffer();

    vec3 L = normalize(in_light.direction);
    vec3 V = normalize(s.position-(view_matrix * vec4(0,0,0,1)).xyz);

    out_image = in_light.intensity * in_light.color * compute_lighting(s, L, V);
}
