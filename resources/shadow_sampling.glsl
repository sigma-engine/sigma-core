#ifndef SHADOW_SAMPLING_GLSL
#define SHADOW_SAMPLING_GLSL

#include <math.glsl>

#define SAMPLES 25
#define SAMPLE_SPREAD 2

float variance_shadow(vec2 moments, float compare)
{
    float p = step(compare, moments.x);
    float variance = max(moments.y - moments.x * moments.x, .009);

    float d = compare - moments.x;
    float p_max = linstep(.2, 1.0, variance / (variance + d * d));

    return min(max(p, p_max), 1.0);
}

float calculate_shadow(sampler2D shadow_map, vec4 light_space_position, float compare)
{
    vec3 ndc_position = (light_space_position.xyz / light_space_position.w) * vec3(.5) + vec3(.5);

    vec2 moments = texture(shadow_map, ndc_position.xy).xy;
    if (moments.x >= 0 && moments.y >= 0) {
        float shadow = variance_shadow(moments, compare);
        vec2 texel_size = vec2(1.0) / textureSize(shadow_map, 0);
        for (uint i = 1; i < SAMPLES; i++) {
            vec2 Xi = (2 * hammersley(i, SAMPLES) - vec2(1)) * SAMPLE_SPREAD;
            vec2 shadow_coords = ndc_position.xy + Xi * texel_size;
            moments = texture(shadow_map, shadow_coords).xy;
            shadow += variance_shadow(moments, compare);
        }

        return shadow / SAMPLES;
    }
    return 1;
}

#endif // SHADOW_SAMPLING_GLSL
