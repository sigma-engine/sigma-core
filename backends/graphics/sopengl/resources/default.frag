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

out vec3 position_output;
out vec3 diffuse_output;
out vec3 normal_output;
out vec3 texture_coordinate_output;

void main()
{
    position_output = vertex_position.rgb;
    diffuse_output = texture(basecolor_map,vertex_texcoord).rgb;
    normal_output = vertex_normal;
    texture_coordinate_output = vec3(vertex_texcoord,0);
}
