#version 330

#include <post_process_effect.glsl>
#include <lighting.glsl>

uniform directional_light light;

void main()
{
    surface s = read_geometry_buffer();

    vec3 L = normalize(light.direction);
    vec3 V = -normalize(s.position);

    vec3 final_color = light.intensity * compute_lighting(s,L,V);

    out_image = vec4(final_color, 1);
}
