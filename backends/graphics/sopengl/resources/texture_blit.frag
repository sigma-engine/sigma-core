#version 330

#include <vertex.glsl>
#include <uniforms.glsl>

uniform sampler2D in_image;

const vec4 gamma_correction = vec4(0.45454545454);

out vec4 out_image;

void main()
{
    vec4 c = vec4(texture(in_image, in_vertex.texcoord).rgb, 1);
    out_image = pow(c, gamma_correction);
}
