#version 400

#include <uniforms.glsl>
#include <vertex.glsl>

layout(location = 0) out float fragment;

void main()
{
    fragment = gl_FragCoord.z;
}
