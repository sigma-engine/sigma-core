#version 330

#include <vertex.glsl>
#include <uniforms.glsl>

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec2 in_texcoord;

void main()
{
    mat3 m3_model_matrix = mat3(model_matrix);
    out_vertex.position = model_matrix * vec4(in_position,1);
    out_vertex.normal = normalize(m3_model_matrix * in_normal);
    out_vertex.tangent = normalize(m3_model_matrix * in_tangent);
    out_vertex.binormal = normalize(cross(out_vertex.tangent, out_vertex.normal));
    out_vertex.texcoord = in_texcoord;
    gl_Position = projection_matrix * view_matrix * out_vertex.position;
}
