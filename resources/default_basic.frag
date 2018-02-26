#version 460

#include <pbr/deffered/geometry_buffer.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>

layout(binding=10) uniform default_basic {
    vec3 basecolor;
    float metalness;
    float roughness;
};

void main()
{
    surface s;
    s.diffuse = basecolor;
    s.metalness = metalness;
    s.roughness = roughness;
    s.normal = normalize(in_vertex.normal);
    write_geometry_buffer(s);
}
