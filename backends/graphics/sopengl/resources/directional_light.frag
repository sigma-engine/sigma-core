#version 330

#include "lighting.glsl"

uniform directional_light light;

uniform sampler2D in_position;
uniform sampler2D in_diffuse;
uniform sampler2D in_normal;

uniform mat4 view_matrix;
uniform mat4 model_matrix;

in vec4 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;
in vec3 vertex_binormal;

out vec4 out_image;

void main()
{

    vec2 uv = calculate_texture_coordinate();
    vec3 surface_position = texture(in_position, uv).xyz;
    vec3 surface_normal = texture(in_normal, uv).xyz;
    vec3 surface_diffuse = texture(in_diffuse, uv).rgb;

    vec3 L = normalize((model_matrix * vec4(0, 0, -1, 0)).xyz); // TODO move to the cpu
    vec3 V = (view_matrix * vec4(0, 0, -1, 0)).xyz; // TODO move to the cpu
    vec3 o = light.intensity * light.color * surface_diffuse * orenNayarDiffuse(L, V, surface_normal, .2, .8);

    out_image = vec4(o, 1);
}
