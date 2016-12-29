#version 330

#include <lighting.glsl>
#include <post_process_effect.glsl>

uniform directional_light light;

void main()
{
    surface s = read_geometry_buffer();

    vec3 L = normalize(light.direction);
    vec3 V = -normalize(s.position);
    vec3 o = light.intensity * light.color * s.diffuse * orenNayarDiffuse(L, V, s.normal, .2, .8);
    //vec3 o = fresnel(vec3(.03), V, s.normal);

    out_image = vec4(o, 1);
}
