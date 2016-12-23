#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 vertex_tangent;
layout(location = 3) in vec2 vertex_texcoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec3 position;
out vec3 normal;
out vec3 tangent;
out vec2 texcoord;
out vec3 binormal;

void main()
{
    position = mat3(modelMatrix) * vertex_position;
    normal = vertex_normal;
    tangent = vertex_tangent;
    binormal = normalize(cross(tangent, normal));
    texcoord = vertex_texcoord;
    gl_Position = projectionMatrix * vec4(mat3(viewMatrix) * vertex_position, 1);
}
