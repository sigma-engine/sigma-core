#version 420

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <pbr/brdf.glsl>
// clang-format on

in point_light
{
    vec3 position;
    float radius;
    vec3 color;
    float intensity;
}
in_light;

void main()
{
    surface s = read_geometry_buffer();

    vec3 V = normalize(eye_position.xyz - s.position);
    vec3 L = in_light.position - s.position;
    float light_distance = length(L);
    L = normalize(L);

    // TODO Use eq (9) from
    // http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
    float att = smoothstep(1, 0, light_distance / in_light.radius) / (light_distance * light_distance);

    out_image = att * in_light.intensity * in_light.color * compute_lighting(s, L, V);
}
