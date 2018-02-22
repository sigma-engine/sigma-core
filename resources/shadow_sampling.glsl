#ifndef SHADOW_SAMPLING_GLSL
#define SHADOW_SAMPLING_GLSL

#include <math.glsl>

#define SAMPLES 25
#define SAMPLE_SPREAD 4

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

float calculate_shadow(sampler2DArrayShadow shadow_map, float layer, vec4 light_space_position, float compare)
{

    vec2 texel_size = vec2(1.0) / textureSize(shadow_map, 0).xy;
    vec3 ndc_position = (light_space_position.xyz / light_space_position.w) * vec3(.5) + vec3(.5);

    float shadow = 0.0;

    /* const float NUM_SAMPLES = 5.0f; */
    /* const float SAMPLES_START = (NUM_SAMPLES - 1) / 2.0f; */
    /* const float SAMPLES_SQUARED = NUM_SAMPLES * NUM_SAMPLES; */
    /* for(float y=-SAMPLES_START;y<=SAMPLES_START;++y) */
    /* { */
    /*     for(float x=-SAMPLES_START;x<=SAMPLES_START;++x) { */
    /*         vec2 offset = vec2(x,y) * texel_size; */
    /*         shadow += step(compare, textureProj(shadow_map, vec4(ndc_position.xy + offset, ndc_position.z - 2*texel_size.y, 1.0))); */
    /*     } */
    /* } */
    /* return shadow / SAMPLES_SQUARED; */
    for (uint i = 0; i < SAMPLES; i++) {
        vec2 Xi = (2 * hammersley(i, SAMPLES) - vec2(1)) * SAMPLE_SPREAD;
        vec2 shadow_coords = ndc_position.xy + Xi * texel_size;
        shadow += texture(shadow_map, vec4(shadow_coords, layer, compare - texel_size.y));
    }

    return shadow / SAMPLES;
}

#endif // SHADOW_SAMPLING_GLSL
