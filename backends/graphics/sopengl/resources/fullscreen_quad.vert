#version 330

#include <vertex.glsl>
#include <uniforms.glsl>

void main()
{
    // TODO this can be removed if the renderer does the correct thing
    mat3 corrected_normal_matrix = transpose(inverse(mat3(view_matrix)));
    out_vertex.position = view_matrix * vec4(in_position,1);
    out_vertex.normal = normalize(corrected_normal_matrix * in_normal);
    out_vertex.tangent = normalize(corrected_normal_matrix * in_tangent);
    out_vertex.binormal = normalize(cross(out_vertex.tangent, out_vertex.normal));
    out_vertex.texcoord = in_texcoord;
    gl_Position = vec4(in_position,1);
}
