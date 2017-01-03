#version 330

#include <geometry_buffer.glsl>
#include <uniforms.glsl>
#include <vertex.glsl>

void main()
{
    surface s;
    s.diffuse = vec3(0, 0, in_vertex.position.y);
    s.normal = in_vertex.normal.xyz;
    write_geometry_buffer(s);
}
