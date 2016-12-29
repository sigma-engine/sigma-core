#ifndef SIGMA_ENGINE_OPENGL_UNIFORM_GLSL
#define SIGMA_ENGINE_OPENGL_UNIFORM_GLSL

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;
uniform float z_near;
uniform float z_far;
uniform vec2 view_port_size;
uniform float time;

#endif // SIGMA_ENGINE_OPENGL_UNIFORM_GLSL
