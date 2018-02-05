#version 460

#include <uniforms.glsl>
#include <vertex.glsl>

// per-instance attributes.
// layout(location = 4) in vec4 color_intensity;
// layout(location = 5) in vec4 position_radius;

uniform vec4 color_intensity;
uniform vec4 position_radius;

out point_light
{
    vec3 position;
    float radius;
    vec3 color;
    float intensity;
}
out_light;

void main()
{
    out_light.position = position_radius.xyz;
    out_light.radius = position_radius.w;
    out_light.color = color_intensity.rgb;
    out_light.intensity = color_intensity.a;

    out_vertex.position = vec4(position_radius.xyz + position_radius.w * in_position, 1);
    out_vertex.normal = normalize(in_normal);
    out_vertex.tangent = normalize(in_tangent);
    out_vertex.bitangent = normalize(in_bitangent);
    out_vertex.texcoord = in_texcoord;

    gl_Position = projection_view_matrix * out_vertex.position;
}
