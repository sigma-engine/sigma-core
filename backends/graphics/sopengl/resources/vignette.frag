#version 330

#include <vertex.glsl>

uniform sampler2D in_image;
uniform vec2 view_port_size;

out vec4 out_image;

void main()
{
    vec4 c = vec4(texture(in_image, in_vertex.texcoord).rgb, 1);
    vec2 rpos = gl_FragCoord.xy / view_port_size - .5;
    float len = length(rpos);
    float vin = smoothstep(.5, .4, len);
    c.rgb = mix(c.rgb, c.rgb * vin, .7);
    out_image = c;
}
