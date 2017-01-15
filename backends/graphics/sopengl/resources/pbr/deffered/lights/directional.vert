#version 400

#include <uniforms.glsl>
#include <vertex.glsl>

// per-instance attributes.
layout(location = 4) in vec4 color_intensity;
layout(location = 5) in vec3 direction;

out directional_light
{
    vec3 color;
    float intensity;
    vec3 direction;
} out_light;

void main()
{
    out_light.color = color_intensity.rgb;
    out_light.intensity = color_intensity.a;
    out_light.direction = (view_matrix * vec4(direction,0)).xyz;

    out_vertex.position = vec4(in_position, 1);
    out_vertex.normal = normalize(in_normal);
    out_vertex.tangent = normalize(in_tangent);
    out_vertex.binormal = normalize(cross(out_vertex.tangent, out_vertex.normal));
    out_vertex.texcoord = in_texcoord;

    gl_Position = out_vertex.position;
}
