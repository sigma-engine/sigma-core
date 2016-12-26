#version 330

uniform sampler2D input_image;

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 texcoord;
in vec3 binormal;

out vec4 output;

void main()
{
    output = vec4(texture(input_image, texcoord).rgb,1);

}
