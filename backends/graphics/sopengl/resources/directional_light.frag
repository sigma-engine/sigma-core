#version 330

#include <lighting.glsl>
#include <post_process_effect.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>

uniform directional_light light;

void main()
{
    vec2 uv = calculate_texture_coordinate();
    vec3 surface_position = texture(in_position, uv).xyz;
    vec3 surface_normal = texture(in_normal, uv).xyz;
    vec3 surface_diffuse = texture(in_diffuse, uv).rgb;

    vec3 L = normalize((model_matrix * vec4(0, 0, -1, 0)).xyz); // TODO move to the cpu
    vec3 V = (view_matrix * vec4(0, 0, -1, 0)).xyz; // TODO move to the cpu
    vec3 o = light.intensity * light.color * surface_diffuse * orenNayarDiffuse(L, V, surface_normal, .2, .8);
    //vec3 o = fresnel(vec3(.03), V, surface_normal);

    out_image = vec4(o, 1);
}
