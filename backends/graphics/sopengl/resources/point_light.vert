#version 330

#include <uniforms.glsl>
#include <vertex.glsl>

layout(location = 4) in vec4 position_radius;
layout(location = 5) in vec4 color_intensity;


out point_light
{
    vec3 position;
    float radius;
    vec3 color;
    float intensity;
} out_light;

void main()
{
    vec4 pos = view_matrix * vec4(position_radius.xyz + position_radius.w*in_position,1);
    out_light.position = (model_view_matrix * vec4(position_radius.xyz + position_radius.w*vec3(0,0,0),1)).xyz;
    out_light.radius = position_radius.w;
    out_light.color = color_intensity.rgb;
    out_light.intensity = color_intensity.a;

    out_vertex.position = pos;
    out_vertex.normal = normalize(normal_matrix * in_normal);
    out_vertex.tangent = normalize(normal_matrix * in_tangent);
    out_vertex.binormal = normalize(cross(out_vertex.tangent, out_vertex.normal));
    out_vertex.texcoord = in_texcoord;

    gl_Position = projection_matrix * pos;
}
