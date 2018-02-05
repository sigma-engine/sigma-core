#ifndef SIGMA_GRAPHICS_OPENGL_UNIFORM_GLSL
#define SIGMA_GRAPHICS_OPENGL_UNIFORM_GLSL

#include <sigma/graphics/standard_block.glsl>

struct instance_matrices {
    mat4 model_matrix_;
    mat4 model_view_matrix_;
    mat4 normal_matrix_;
};

layout (std140, binding = 2) buffer instance_matrices_block {
    instance_matrices data[];
} matrix_instances;

#define model_matrix matrix_instances.data[gl_DrawID].model_matrix_
#define model_view_matrix matrix_instances.data[gl_DrawID].model_view_matrix_
#define normal_matrix mat3(matrix_instances.data[gl_DrawID].normal_matrix_)

// #ifdef SIGMA_ENGINE_VERTEX_SHADER
// // per-instance attributes.
// layout(location = 4) in mat4 model_matrix;
// #endif // SIGMA_ENGINE_VERTEX_SHADER

#endif // SIGMA_GRAPHICS_OPENGL_UNIFORM_GLSL
