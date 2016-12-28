#version 330

#include <lighting.glsl>
#include <post_process_effect.glsl>

uniform point_light light;

void main()
{

    surface s = read_geometry_buffer();

    vec3 L = light.position - s.position;
    float light_distance = length(L);
    L = normalize(L);

    float att = attenuation(light.radius, light.falloff, light_distance);

    vec3 V = -normalize(s.position);
    vec3 o = light.intensity * light.color * s.diffuse * orenNayarDiffuse(L, V, s.normal, .2, .8) * att;
    //vec3 o = fresnel(vec3(.03), L, s.normal) * att;

    out_image = vec4(o,1);
}
