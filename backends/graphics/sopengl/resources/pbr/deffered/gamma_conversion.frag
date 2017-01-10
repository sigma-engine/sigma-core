#version 330

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>
// clang-format on

const vec4 gamma_correction = vec4(1.0 / 2.2);

uniform samplerCube cubemap_texture;

void main()
{
    surface s = read_geometry_buffer();

    vec3 V = normalize((inverse(view_matrix) * vec4(-s.position, 0.0)).xyz);
    if(s.depth == 1.0) {
        out_image = texture(cubemap_texture, -V, 0.0);
    }
    else {
        vec4 c = vec4(texture(in_image, in_vertex.texcoord).rgb,1);
        out_image = pow(c, gamma_correction);
    }
}
