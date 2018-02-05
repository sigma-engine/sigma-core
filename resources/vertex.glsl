#ifndef SIGMA_GRAPHICS_OPENGL_VERTEX_GLSL
#define SIGMA_GRAPHICS_OPENGL_VERTEX_GLSL

#if defined(SIGMA_ENGINE_VERTEX_SHADER)
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_bitangent;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec2 in_texcoord;

layout(location = 0) out vertex_data
{
    vec4 position;
    vec3 normal;
    vec3 tangent;
    vec2 texcoord;
    vec3 bitangent;
}
out_vertex;
#elif defined(SIGMA_ENGINE_FRAGMENT_SHADER)
layout(location = 0) in vertex_data
{
    vec4 position;
    vec3 normal;
    vec3 tangent;
    vec2 texcoord;
    vec3 bitangent;
}
in_vertex;
#elif defined(SIGMA_ENGINE_GEOMETRY_SHADER)
layout(location = 0) in vertex_data
{
    vec4 position;
    vec3 normal;
    vec3 tangent;
    vec2 texcoord;
    vec3 bitangent;
}
in_vertex[];

layout(location = 0) out vertex_data
{
    vec4 position;
    vec3 normal;
    vec3 tangent;
    vec2 texcoord;
    vec3 bitangent;
}
out_vertex;
#else
#error "This shader type is not supported"
#endif

#endif // SIGMA_GRAPHICS_OPENGL_VERTEX_GLSL
