#version 330

#include <geometry_buffer.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>

uniform sampler2D basecolor_map;
uniform sampler2D metallic_map;
uniform sampler2D normal_map;
uniform sampler2D roughness_map;

void main()
{
    surface s;
    // TODO do this conversion on the cpu
    s.diffuse = pow(texture(basecolor_map, in_vertex.texcoord).rgb, vec3(2.2));
    s.metalness = texture(metallic_map, in_vertex.texcoord).r;
    s.roughness = texture(roughness_map, in_vertex.texcoord).r;
    s.normal = in_vertex.normal.xyz; // TODO normal map
    write_geometry_buffer(s);
}
