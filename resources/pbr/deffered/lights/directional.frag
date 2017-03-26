#version 400

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <pbr/brdf.glsl>
#include <shadow_sampling.glsl>
// clang-format on

in directional_light
{
    vec3 color;
    float intensity;
    vec3 direction;
    vec3 center;
} in_light;

void main()
{
    surface s = read_geometry_buffer();

    vec3 V = normalize(eye_position - s.position);
    vec3 L = in_light.direction;

    vec4 light_space_position = light_projection_view_matrix * vec4(s.position,1);

    float depth = length(s.position - in_light.center);
    float shadow = calculate_shadow(in_shadow_map, light_space_position, depth); // bias = 0.0004

    out_image = shadow * in_light.intensity * in_light.color * compute_lighting(s, L, V);
}
