#ifndef SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL
#define SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL

#include <uniforms.glsl>

struct surface {
    vec3 position;
    vec3 diffuse;
    vec3 normal;
};

#if defined(SIGMA_ENGINE_POST_PROCESS_EFFECT)
uniform sampler2D in_diffuse;
uniform sampler2D in_normal;
uniform sampler2D in_image;
uniform sampler2D in_depth_stencil;

out vec4 out_image;

surface read_geometry_buffer()
{
    vec2 uv = gl_FragCoord.xy/view_port_size;

    surface s;
    float depth = 2*texture(in_depth_stencil, uv).x - 1;
    vec4 view_space_position = inverse(projection_matrix)*vec4(2 * uv - 1,depth,1);
    s.position = (view_space_position/view_space_position.w).xyz;
    s.normal = texture(in_normal, uv).xyz;
    s.diffuse = texture(in_diffuse, uv).rgb;
    return s;
}
#else

out vec3 diffuse_output;
out vec3 normal_output;

void write_geometry_buffer(surface s)
{
    diffuse_output = s.diffuse;
    normal_output = s.normal;
}
#endif

#endif // SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL
