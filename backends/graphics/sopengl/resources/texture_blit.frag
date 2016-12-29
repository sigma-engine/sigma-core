#version 330

#include <lighting.glsl>
#include <post_process_effect.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>

const vec4 gamma_correction = vec4(0.45454545454);

void main()
{
    vec4 c = vec4(texture(in_image, in_vertex.texcoord).rgb, 1);
    out_image = pow(c, gamma_correction);
}
