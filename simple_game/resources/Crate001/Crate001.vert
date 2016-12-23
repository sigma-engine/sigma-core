#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 texcoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform float time;

out vec3 vertex_position;
out vec3 vertex_normal;
out vec3 vertex_tangent;
out vec2 vertex_texcoord;
out vec3 vertex_binormal;

void main()
{
    float scale = .8 * (cos(time) + 2.0);
    vertex_position = (modelMatrix * vec4(position, 1)).xyz;
    vertex_normal = normalize((modelMatrix * vec4(normal, 0))).xyz;
    vertex_tangent = normalize((modelMatrix * vec4(tangent, 0)).xyz);
    vertex_binormal = normalize(cross(vertex_tangent, vertex_normal));
    vertex_texcoord = texcoord;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1);
    // gl_Position  =  (modelMatrix * viewMatrix * vec4(position,1)) *
    // projectionMatrix;
}
