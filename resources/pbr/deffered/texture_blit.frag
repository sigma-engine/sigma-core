#version 460

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <vertex.glsl>
// clang-format on

void main()
{
    out_image = texture(in_image, in_vertex.texcoord).rgb;
}
