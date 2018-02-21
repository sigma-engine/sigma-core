#ifndef SIGMA_PBR_DEFFERED_GEOMETRY_BUFFER_GLSL
#define SIGMA_PBR_DEFFERED_GEOMETRY_BUFFER_GLSL

#include <uniforms.glsl>

#define MIN_ROUGHNESS .08

struct surface {
#if defined(SIGMA_PBR_DEFFERED_POST_PROCESS_EFFECT)
    vec3 position;
    float depth;
#endif // SIGMA_PBR_DEFFERED_POST_PROCESS_EFFECT
    vec3 normal;
    vec3 diffuse;
    float roughness;
    float metalness;
};

#if defined(SIGMA_PBR_DEFFERED_POST_PROCESS_EFFECT)
uniform sampler2D in_diffuse_roughness;
uniform sampler2D in_normal_metalness;
uniform sampler2D in_depth_stencil;
uniform sampler2D in_image;
uniform sampler2DArrayShadow in_shadow_map;

layout(location = 0) out vec3 out_image;

surface read_geometry_buffer()
{
    surface s;

    vec2 uv = gl_FragCoord.xy / view_port_size;

    float depth = texture(in_depth_stencil, uv).x;
    vec4 diffuse_roughness = texture(in_diffuse_roughness, uv);
    vec4 normal_metalness = texture(in_normal_metalness, uv);

    vec4 world_space_position = inverse_projection_view_matrix * vec4(2 * uv - 1, 2 * depth - 1, 1);

    s.position = (world_space_position / world_space_position.w).xyz;
    s.depth = depth;
    s.normal = normalize(normal_metalness.xyz); // TODO not sure why this needs to be normalized???

    s.diffuse = diffuse_roughness.rgb;
    s.roughness = diffuse_roughness.a;
    s.metalness = normal_metalness.a;

    return s;
}
#else

#ifdef SIGMA_ENGINE_TRANSPARENT
uniform sampler2D in_image;
#endif

layout(location = 0) out vec4 out_diffuse_roughness;
layout(location = 1) out vec4 out_normal_metalness;

void write_geometry_buffer(surface s)
{
    out_diffuse_roughness = vec4(s.diffuse, s.roughness + MIN_ROUGHNESS);
    out_normal_metalness = vec4(s.normal, s.metalness);
}
#endif

#endif // SIGMA_PBR_DEFFERED_GEOMETRY_BUFFER_GLSL
