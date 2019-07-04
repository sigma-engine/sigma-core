#version 450

// layout(location = 0) in vec3 position;
// layout(location = 1) in vec3 color;

// #extension GL_KHR_vulkan_glsl : enable

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5));

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0));

layout(location = 0) out vec3 vColor;

void main()
{
    // vColor = color;
    // gl_Position = vec4(position, 1);
    vColor = colors[gl_VertexIndex];
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}
