#version 330

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;
in vec3 vertex_binormal;

void main()
{
    gl_FragColor = texture2D(diffuse_map, vertex_texcoord);
}
