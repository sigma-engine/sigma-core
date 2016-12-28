#version 330

uniform sampler2D basecolor_map;
uniform sampler2D metallic_map;
uniform sampler2D normal_map;
uniform sampler2D roughness_map;

in vec4 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;
in vec3 vertex_binormal;

out vec4 position_output;
out vec4 diffuse_output;
out vec4 normal_output;

void main()
{
    position_output = vertex_position;
    // TODO this should be done by sctexture
    diffuse_output = pow(texture(basecolor_map,vertex_texcoord),vec4(2.2));
    normal_output = vec4(vertex_normal,1);
}
