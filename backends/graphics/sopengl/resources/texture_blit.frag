#version 330

uniform sampler2D input_image;

in vec4 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;
in vec3 vertex_binormal;

out vec4 output;

const vec4 gamma_correction = vec4(0.45454545454);

void main()
{
    vec4 c = vec4(texture(input_image, vertex_texcoord).rgb,1);
    output = pow(c,gamma_correction);
}
