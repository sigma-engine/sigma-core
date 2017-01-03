#version 330

// clang-format off
#include <post_process_effect.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>
// clang-format on

const vec4 gamma_correction = vec4(0.45454545454);

void main()
{
    surface s = read_geometry_buffer();

    vec4 c = texture(in_image, in_vertex.texcoord);
    out_image = pow(c, gamma_correction);
    out_image.a = 1;
}
