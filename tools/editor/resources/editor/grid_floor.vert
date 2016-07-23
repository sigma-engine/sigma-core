#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 texcoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform float time;

out vec4 vertex_position;
out vec4 final;

void main()
{
    vertex_position = vec4(position, 1);
    final = projectionMatrix * viewMatrix * vertex_position;
    gl_Position = final;
}
