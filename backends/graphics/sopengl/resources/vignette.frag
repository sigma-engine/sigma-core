#version 330

uniform sampler2D in_image;
uniform vec2 view_port_size;

in vec4 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;
in vec3 vertex_binormal;

out vec4 out_image;

void main()
{
    vec4 c = vec4(texture(in_image, vertex_texcoord).rgb, 1);
    vec2 rpos = gl_FragCoord.xy / view_port_size - .5;
    float len = length(rpos);
    float vin = smoothstep(.5, .4, len);
    c.rgb = mix(c.rgb, c.rgb * vin, .7);
    out_image = c;
}
