#version 420

#define SIGMA_ENGINE_TRANSPARENT
#include <pbr/deffered/geometry_buffer.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>

uniform vec4 color;
uniform float roughness;

void main()
{
    vec2 uv = gl_FragCoord.xy / view_port_size;

    surface s;

    vec3 background_color = texture(in_image, uv).rgb;
    s.diffuse = mix(color.rgb * background_color, color.rgb, color.a);
    s.metalness = 0;
    s.roughness = roughness;
    s.normal = normalize(in_vertex.normal);
    write_geometry_buffer(s);
}
