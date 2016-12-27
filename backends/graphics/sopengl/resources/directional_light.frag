#version 330

#include "lighting.glsl"

uniform vec3 color;
uniform vec3 direction;

in vec4 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;
in vec3 vertex_binormal;

out vec4 output;

void main()
{
    output = vec4(1,0,0,1);
}
