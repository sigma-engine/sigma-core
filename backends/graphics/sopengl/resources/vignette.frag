#version 400

#include <pbr/deffered/post_process_effect.glsl>

void main()
{
    vec3 c = texture(in_image, in_vertex.texcoord).rgb;
    vec2 rpos = gl_FragCoord.xy / view_port_size - .5;
    float len = length(rpos);
    float vin = smoothstep(.5, .4, len);
    c = mix(c, c * vin, .7);
    out_image = c;
}
