#ifndef SIGMA_GRAPHICS_OPENGL_UNIFORM_GLSL
#define SIGMA_GRAPHICS_OPENGL_UNIFORM_GLSL

#include <sigma/graphics/standard_block.glsl>

layout (std140, binding = 2) uniform instance_matrices {
    mat4 model_matrix;
    mat4 model_view_matrix;
    mat3 normal_matrix;
};

// #ifdef SIGMA_ENGINE_VERTEX_SHADER
// // per-instance attributes.
// layout(location = 4) in mat4 model_matrix;
// #endif // SIGMA_ENGINE_VERTEX_SHADER

#endif // SIGMA_GRAPHICS_OPENGL_UNIFORM_GLSL
