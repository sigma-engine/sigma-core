#version 420

#include <uniforms.glsl>
#include <vertex.glsl>

// per-instance attributes.
// layout(location = 4) in vec4 color_intensity;
// layout(location = 5) in vec4 position_radius;

uniform vec4 color_intensity;
uniform vec3 position;
uniform vec3 direction;
uniform float cutoff;

out spot_light
{
    vec3 color;
    float intensity;
    vec3 position;
    vec3 direction;
    float cutoff;
}
out_light;

void main()
{
    out_light.color = color_intensity.rgb;
    out_light.intensity = color_intensity.a;
    out_light.position = position;
    out_light.direction = normalize(direction);
    out_light.cutoff = cutoff;

    out_vertex.position = vec4(in_position, 1);
    out_vertex.normal = normalize(in_normal);
    out_vertex.tangent = normalize(in_tangent);
    out_vertex.bitangent = normalize(cross(out_vertex.tangent, out_vertex.normal));
    out_vertex.texcoord = in_texcoord;

    gl_Position = out_vertex.position;
    // out_vertex.position = vec4(position_radius.xyz + position_radius.w * in_position, 1);
    // out_vertex.normal = normalize(in_normal);
    // out_vertex.tangent = normalize(in_tangent);
    // out_vertex.bitangent = normalize(cross(out_vertex.tangent, out_vertex.normal));
    // out_vertex.texcoord = in_texcoord;

    // gl_Position = projection_view_matrix * out_vertex.position;
}
