#ifndef SHADOW_SAMPLING_GLSL
#define SHADOW_SAMPLING_GLSL

#include <math.glsl>

#define SAMPLES 25
#define SAMPLE_SPREAD 2

bool check_coords(vec2 shadow_coords)
{
    return shadow_coords.x >= 0 && shadow_coords.x <= 1 && shadow_coords.y >= 0 && shadow_coords.y <= 1;
}

float variance_shadow(vec2 moments, float compare)
{
    float p = step(compare, moments.x);
    float variance = max(abs(moments.y - moments.x * moments.x), 0.00002);

    float d = compare - moments.x;
    float p_max = linstep(0.10, 2.0, variance / (variance + d * d));

    return min(max(p, p_max), 1.0);
}

float calculate_shadow(sampler2D shadow_map, vec4 light_space_position, float compare)
{
    vec2 texel_size = vec2(1.0) / textureSize(shadow_map, 0);
    vec3 ndc_position = (light_space_position.xyz / light_space_position.w) * vec3(.5) + vec3(.5);

    float shadow = 0.0;
    for (uint i = 0; i < SAMPLES; i++) {
        vec2 Xi = (2 * hammersley(i, SAMPLES) - vec2(1)) * SAMPLE_SPREAD;
        vec2 shadow_coords = ndc_position.xy + Xi * texel_size;
        if (check_coords(shadow_coords)) {
            shadow += variance_shadow(texture(shadow_map, shadow_coords).xy, compare);
        } else {
            shadow += 1;
        }
    }

    return shadow / SAMPLES;
}

#endif // SHADOW_SAMPLING_GLSL
