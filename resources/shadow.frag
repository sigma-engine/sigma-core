#version 460

#include <uniforms.glsl>
#include <vertex.glsl>

layout(location = 0) out vec2 fragment;

void main()
{
    vec4 viewspace = projection_view_matrix * in_vertex.position;
    float depth = (viewspace.z / viewspace.w) *  0.5 + 0.5;

    float dx = dFdx(depth);
    float dy = dFdy(depth);
    fragment = vec2(depth, depth * depth + 0.25 * (dx * dx + dy * dy));
}
