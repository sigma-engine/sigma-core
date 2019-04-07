#ifndef SIGMA_GRAPHICS_STANDARD_BLOCK_GLSL
#define SIGMA_GRAPHICS_STANDARD_BLOCK_GLSL

layout (std140, binding = 0) uniform standard_block {

    mat4 projection_matrix;
    mat4 inverse_projection_matrix;

    mat4 view_matrix;
    mat4 inverse_view_matrix;

    mat4 projection_view_matrix;
    mat4 inverse_projection_view_matrix;

    vec4 eye_position;
    vec2 view_port_size;

    float time;

    float fovy;
    float z_near;
    float z_far;
};

#endif // SIGMA_GRAPHICS_STANDARD_BLOCK_GLSL
