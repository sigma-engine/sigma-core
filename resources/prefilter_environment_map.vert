#version 420
#include <vertex.glsl>

uniform mat4 model_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;

void main()
{
    out_vertex.position = normalize(vec4(in_position, 1));
    gl_Position = model_view_matrix * out_vertex.position;
}
