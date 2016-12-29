#ifndef SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL
#define SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL

#include <uniforms.glsl>

struct surface {
    vec3 position;
    vec3 diffuse;
    vec3 normal;
};

#if defined(SIGMA_ENGINE_POST_PROCESS_EFFECT)
uniform sampler2D in_position;
uniform sampler2D in_diffuse;
uniform sampler2D in_normal;
uniform sampler2D in_image;
out vec4 out_image;

vec2 calculate_texture_coordinate()
{
    return gl_FragCoord.xy / view_port_size;
}

surface read_geometry_buffer()
{
    vec2 uv = calculate_texture_coordinate();

    surface s;
    s.position = texture(in_position, uv).xyz;
    s.normal = texture(in_normal, uv).xyz;
    s.diffuse = texture(in_diffuse, uv).rgb;
    return s;
}
#else
out vec3 position_output;
out vec3 diffuse_output;
out vec3 normal_output;
void write_geometry_buffer(surface s)
{
    position_output = s.position;
    diffuse_output = s.diffuse;
    normal_output = s.normal;
}
#endif

#endif // SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL
