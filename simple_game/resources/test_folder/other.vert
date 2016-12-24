#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 vertex_tangent;
layout(location = 3) in vec2 vertex_texcoord;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

out vec3 normal;

void main()
{
    normal = normalMatrix * vertex_normal;
    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex_position, 1);
}
