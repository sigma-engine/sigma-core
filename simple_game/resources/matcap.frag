#version 330

uniform sampler2D matcap;

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;
in vec3 vertex_binormal;

in vec3 eye_vector;
in vec3 n;

/*layout (location = 0) out vec3 a;
layout (location = 1) out vec3 DiffuseOut;
layout (location = 2) out vec3 b;
layout (location = 3) out vec3 c;*/

void main()
{
    vec3 r = reflect(eye_vector, n);
    float m = 2.0 * length(vec3(0, 0, 1.1) + r);
    vec2 uv = r.xy / m + 0.5;

    gl_FragColor = vec4(texture2D(matcap, uv).rgb, 1.);
}
