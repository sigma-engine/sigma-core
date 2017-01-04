#ifndef SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL
#define SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL

#include <uniforms.glsl>

#define MIN_ROUGHNESS .08

struct surface {
#if defined(SIGMA_ENGINE_POST_PROCESS_EFFECT)
    vec3 position;
#endif // SIGMA_ENGINE_POST_PROCESS_EFFECT
    vec3 normal;
    vec3 diffuse;
    float roughness;
    float metalness;
};

#if defined(SIGMA_ENGINE_POST_PROCESS_EFFECT)
uniform sampler2D in_diffuse_roughness;
uniform sampler2D in_normal_metalness;
uniform sampler2D in_depth_stencil;
uniform sampler2D in_image;

out vec4 out_image;

surface read_geometry_buffer()
{
    surface s;

    vec2 uv = gl_FragCoord.xy / view_port_size;

    float depth = texture(in_depth_stencil, uv).x;
    vec4 diffuse_roughness = texture(in_diffuse_roughness, uv);
    vec4 normal_metalness = texture(in_normal_metalness, uv);

    // TODO pass inverse projection from cpu
    vec4 view_space_position = inverse(projection_matrix) * vec4(2 * uv - 1, 2 * depth - 1, 1);

    s.position = (view_space_position / view_space_position.w).xyz;
    s.normal = normal_metalness.xyz;

    s.diffuse = diffuse_roughness.rgb;
    s.roughness = diffuse_roughness.a;
    s.metalness = normal_metalness.a;

    return s;
}
#else

out vec4 out_diffuse_roughness;
out vec4 out_normal_metalness;

void write_geometry_buffer(surface s)
{
    float r = MIN_ROUGHNESS+s.roughness;
    out_diffuse_roughness = vec4(s.diffuse, r*r);
    out_normal_metalness = vec4(s.normal, s.metalness);
}
#endif

#endif // SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL
