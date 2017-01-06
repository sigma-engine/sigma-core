#ifndef SIGMA_ENGINE_OPENGL_UNIFORM_GLSL
#define SIGMA_ENGINE_OPENGL_UNIFORM_GLSL

// layout (std140) uniform standard_uniforms {
//     mat4 projection_matrix;
//     mat4 view_matrix;
//     vec2 view_port_size;
//     float time;
//     float z_near;
//     float z_far;
// };

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec2 view_port_size;
uniform float time;
uniform float z_near;
uniform float z_far;


uniform mat4 model_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;


#endif // SIGMA_ENGINE_OPENGL_UNIFORM_GLSL
