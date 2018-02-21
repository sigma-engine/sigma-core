#version 460

#include <uniforms.glsl>
#include <vertex.glsl>

void main()
{
    out_vertex.position = vec4(in_position, 1);
    out_vertex.normal = normalize(in_normal);
    out_vertex.tangent = normalize(in_tangent);
    out_vertex.bitangent = normalize(in_bitangent);
    out_vertex.texcoord = in_texcoord;

    gl_Position = out_vertex.position;
}
