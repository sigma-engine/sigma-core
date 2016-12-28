#version 330

uniform sampler2D in_image;

in vec4 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;
in vec3 vertex_binormal;

const vec4 gamma_correction = vec4(0.45454545454);

out vec4 out_image;

void main()
{
    vec4 c = vec4(texture(in_image, vertex_texcoord).rgb, 1);
    out_image = pow(c, gamma_correction);
}
