#ifndef SHADOW_SAMPLING_GLSL
#define SHADOW_SAMPLING_GLSL

#include <math.glsl>

#define SAMPLES 25
#define SAMPLE_SPREAD 2

float variance_shadow(sampler2D shadow_map, vec2 shadow_coords, float compare)
{
    vec2 moments = texture(shadow_map, shadow_coords).xy;
    if (moments.x >= 0) { // The shadow map was rendered to
        float p = step(compare, moments.x);
        float variance = max(moments.y - moments.x * moments.x, .0009);

        float d = compare - moments.x;
        float p_max = linstep(.05, 1.0, variance / (variance + d * d));

        return min(max(p, p_max), 1.0);
    }
    return 1;
}

float calculate_shadow(sampler2D shadow_map, vec4 light_space_position, float compare)
{
    vec3 ndc_position = (light_space_position.xyz / light_space_position.w) * vec3(.5) + vec3(.5);
    if (ndc_position.z > 0 && ndc_position.z < 1 && ndc_position.x > 0 && ndc_position.y < 1 && ndc_position.y > 0 && ndc_position.y < 1) {
        float shadow = 0;

        vec2 texel_size = vec2(1.0) / textureSize(shadow_map, 0);
        for (uint i = 0; i < SAMPLES; i++) {
            vec2 Xi = (2 * hammersley(i, SAMPLES) - vec2(1)) * SAMPLE_SPREAD;
            shadow += variance_shadow(shadow_map, ndc_position.xy + Xi * texel_size, compare);
        }

        return shadow / SAMPLES;
    }
    return 1;
}

#endif // SHADOW_SAMPLING_GLSL
