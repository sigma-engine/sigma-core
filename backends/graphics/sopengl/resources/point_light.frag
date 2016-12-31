#version 330

#include <post_process_effect.glsl>
#include "lighting.glsl"

uniform point_light light;

void main()
{
    surface s = read_geometry_buffer();

    vec3 V = -normalize(s.position);
    vec3 L = light.position - s.position;
    float light_distance = length(L);
    L = normalize(L);

    float att = smoothstep(1, 0, light_distance / light.radius) / (light_distance * light_distance);

    vec3 final_color = att * light.intensity * compute_lighting(s,L,V);


    out_image = vec4(final_color, 1);
}
