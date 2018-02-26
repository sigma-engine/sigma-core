#version 460

#include <pbr/deffered/post_process_effect.glsl>

layout(binding=10) uniform vignette {
    float inner_radius;
    float outer_radius;
    float opacity;
    vec3 color;
};

void main()
{
    vec3 in_color = texture(in_image, in_vertex.texcoord).rgb;
    vec2 screen_position = 2*in_vertex.texcoord - 1.0;
    float intensity = smoothstep(inner_radius, outer_radius, length(screen_position));
    out_image = mix(in_color, color * opacity + (1 - opacity) * in_color, intensity);
}
