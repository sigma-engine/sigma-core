#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec2 in_texcoord;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;

out vec4 vertex_position;
out vec3 vertex_normal;
out vec3 vertex_tangent;
out vec2 vertex_texcoord;
out vec3 vertex_binormal;

void main()
{
    mat3 m3_model_matrix = mat3(model_matrix);
    vertex_position = model_matrix * vec4(in_position,1);
    vertex_normal = normalize(m3_model_matrix * in_normal);
    vertex_tangent = normalize(m3_model_matrix * in_tangent);
    vertex_binormal = normalize(cross(vertex_tangent, vertex_normal));
    vertex_texcoord = in_texcoord;
    gl_Position = projection_matrix * view_matrix * vertex_position;
}
