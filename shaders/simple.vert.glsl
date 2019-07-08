#version 450

layout(binding = 0) uniform SimpleBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec2 outTexCoord;

void main()
{
	gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPosition, 1.0);
	outTexCoord = inTexCoord;
	outColor = inColor;
}