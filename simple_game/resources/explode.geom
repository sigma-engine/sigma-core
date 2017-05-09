#version 400

#include <uniforms.glsl>
#include <vertex.glsl>

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

vec3 calculate_normal()
{
    vec3 a = in_vertex[2].position.xyz - in_vertex[1].position.xyz;
    vec3 b = in_vertex[0].position.xyz - in_vertex[1].position.xyz;
    return normalize(cross(a, b));
}

void main()
{
    vec3 N = calculate_normal();
    float displacement = .025f;
    for (int i = 0; i < 3; ++i) {
        out_vertex.position = in_vertex[i].position + vec4(displacement * N, 0);
        out_vertex.normal = in_vertex[i].normal;
        out_vertex.tangent = in_vertex[i].tangent;
        out_vertex.texcoord = in_vertex[i].texcoord;
        out_vertex.bitangent = in_vertex[i].bitangent;
        gl_Position = projection_matrix * out_vertex.position;
        EmitVertex();
    }
    EndPrimitive();
}
