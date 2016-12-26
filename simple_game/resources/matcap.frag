#version 330

uniform sampler2D matcap;

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;
in vec3 vertex_binormal;

in vec3 eye_vector;
in vec3 n;

out vec3 diffuse_output;

vec2 matcap_uv(vec3 eye_vec, vec3 normal)
{
    vec3 r = reflect(eye_vec, normal);

    float m = 2.0 * length(vec3(0, 0, 1.1) + r);
    vec2 uv = r.xy / m + 0.5;
    return uv;
}

void main()
{
	diffuse_output = texture2D(matcap, matcap_uv(eye_vector,n)).rgb;
}
