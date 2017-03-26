#version 400

#include <uniforms.glsl>
#include <vertex.glsl>

layout(location = 0) out vec2 fragment;

void main()
{
    float depth = length((view_matrix * in_vertex.position).xyz);
    float dx = dFdx(depth);
    float dy = dFdy(depth);
    fragment = vec2(depth, depth * depth + 0.25*(dx * dx + dy * dy));
}

