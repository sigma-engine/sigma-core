#version 330

#include <lighting.glsl>
#include <post_process_effect.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>

const vec4 gamma_correction = vec4(0.45454545454);


void main()
{
    vec4 c = texture(in_image, in_vertex.texcoord);
    out_image = pow(c, gamma_correction);
    out_image.a = 1;
}
