#version 460

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <pbr/brdf.glsl>
// clang-format on

layout(std140, binding = 3) uniform point_light_block {
    vec4 color_intensity;
    vec4 position_radius;
};

void main()
{
    surface s = read_geometry_buffer();

    vec3 V = normalize(eye_position.xyz - s.position);
    vec3 L = position_radius.xyz - s.position;
    float light_distance = length(L);
    L = normalize(L);

    // TODO Use eq (9) from
    // http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
    float att = smoothstep(1, 0, light_distance / position_radius.w) / (light_distance * light_distance);

    out_image = att * color_intensity.w * color_intensity.xyz * compute_lighting(s, L, V);
}
