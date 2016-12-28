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
    position_output = in_vertex.position;
    // TODO this should be done by sctexture
    diffuse_output = pow(texture(basecolor_map, in_vertex.texcoord), vec4(2.2));
    normal_output = vec4(in_vertex.normal, 1);
}
