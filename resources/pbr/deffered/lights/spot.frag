#version 420

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <pbr/brdf.glsl>
#include <shadow_sampling.glsl>
#include <sigma/graphics/shadow_block.glsl>
// clang-format on

in spot_light
{
    vec3 color;
    float intensity;
    vec3 position;
    vec3 direction;
    float cutoff;
}
in_light;

void main()
{
    surface s = read_geometry_buffer();

    vec3 V = normalize(eye_position - s.position);
    vec3 L = in_light.position - s.position;
    float light_distance = length(L);
    L = normalize(L);

    // TODO Use eq (9) from
    // http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
    // float att = smoothstep(1, 0, light_distance / in_light.radius) / (light_distance * light_distance);
    float att = 1.0 / (light_distance * light_distance);

    float factor = dot(L, in_light.direction);

    if (factor > in_light.cutoff) {
        att *= (1.0 - (1.0 - factor) * 1.0 / (1.0 - in_light.cutoff));
    } else {
        att = 0;
    }

    vec4 light_space_position = light_projection_view_matrix[0] * vec4(s.position, 1);

    vec3 ndc_position = (light_space_position.xyz / light_space_position.w) * vec3(.5) + vec3(.5);
    vec2 shadow_coords = ndc_position.xy;
    float current_depth = ndc_position.z;

    float shadow = calculate_shadow(in_shadow_maps[0], light_space_position, current_depth);

    out_image = shadow * att * in_light.intensity * in_light.color * compute_lighting(s, L, V);
}
