#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 texcoord;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

uniform float time;

out vec3 vertex_position;
out vec3 vertex_normal;
out vec3 vertex_tangent;
out vec2 vertex_texcoord;
out vec3 vertex_binormal;

out vec3 eye_vector;
out vec3 n;

void main()
{
    vec4 p = vec4(position, 1.0);

    eye_vector = normalize(vec3(modelViewMatrix * p));
    n = normalize(normalMatrix * normal);

    gl_Position = projectionMatrix * modelViewMatrix * p;
}
