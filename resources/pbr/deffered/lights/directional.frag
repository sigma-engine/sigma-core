#version 460

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <pbr/brdf.glsl>
#include <shadow_sampling.glsl>
#include <sigma/graphics/shadow_block.glsl>
// clang-format on

layout (std140, binding = 3) uniform directional_light_block {
    vec4 color_intensity;
    vec4 direction_layer;
};

void main()
{
    surface s = read_geometry_buffer();

    vec3 V = normalize(eye_position.xyz - s.position);
    vec3 L = direction_layer.xyz;

    float shadow = 0;
    for (int i = 0; i < 3; ++i) {
        if (plane_point_distance(light_frustum_far_plane[i], s.position) >= 0) {
            vec4 light_space_position = light_projection_view_matrix[i] * vec4(s.position, 1);

            vec3 ndc_position = (light_space_position.xyz / light_space_position.w) * vec3(.5) + vec3(.5);
            vec2 shadow_coords = ndc_position.xy;
            float current_depth = ndc_position.z;

            shadow = calculate_shadow(in_shadow_map, direction_layer.w + i, light_space_position, current_depth);

            break;
        }
    }

    out_image = shadow * color_intensity.xyz * color_intensity.w * compute_lighting(s, L, V);
}
