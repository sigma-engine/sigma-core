#ifndef SHADOW_SAMPLING_GLSL
#define SHADOW_SAMPLING_GLSL

float calculate_shadow(sampler2D shadow_map, vec4 light_space_position)
{
    vec3 ndc_position = (light_space_position.xyz / light_space_position.w) * vec3(.5) + vec3(.5);

    float shadow = 0;

    vec2 texel_size = vec2(1.0) / textureSize(shadow_map, 0);

    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float depth = texture(shadow_map, ndc_position.xy + vec2(x, y) * texel_size).r;
            shadow += ndc_position.z > depth ? 0.0 : 1.0;
        }
    }

    return shadow / 25;
}

#endif // SHADOW_SAMPLING_GLSL
