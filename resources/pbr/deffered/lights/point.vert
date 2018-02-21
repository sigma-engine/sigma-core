#version 460

#include <uniforms.glsl>
#include <vertex.glsl>

layout(std140, binding = 3) uniform point_light_block {
    vec4 color_intensity;
    vec4 position_radius;
};

void main()
{
    out_vertex.position = vec4(position_radius.xyz + position_radius.w * in_position, 1);
    out_vertex.normal = normalize(in_normal);
    out_vertex.tangent = normalize(in_tangent);
    out_vertex.bitangent = normalize(in_bitangent);
    out_vertex.texcoord = in_texcoord;

    gl_Position = projection_view_matrix * out_vertex.position;
}
