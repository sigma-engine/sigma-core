#version 330

// clang-format off
#include <post_process_effect.glsl>
#include <vertex.glsl>
// clang-format on

void main()
{
    out_image = texture(in_image, in_vertex.texcoord);
}
