#version 420
#include <vertex.glsl>
#include <pbr/brdf.glsl>
#include <math.glsl>


uniform sampler2D environment_map;

layout(location = 0) out vec3 out_image;

void main() {
    vec3 N = normalize(in_vertex.position.xyz);
    out_image = textureEquirectangularLod(environment_map, N, 0.0).rgb;
}
