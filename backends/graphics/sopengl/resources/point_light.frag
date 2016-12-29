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

    float att = 1.0 / (light_distance * light_distance); //attenuation(light.radius, .26, light_distance);
    att = att * smoothstep(1, 0, light_distance / light.radius);

    vec3 V = -normalize(s.position);
    vec3 o = (light.intensity * light.color * s.diffuse * orenNayarDiffuse(L, V, s.normal, .2, .8)) * att;
    //vec3 o = fresnel(vec3(.03), L, s.normal) * att;

    out_image = vec4(o, 1);
}
