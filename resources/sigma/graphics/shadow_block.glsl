#ifndef SIGMA_GRAPHICS_SHADOW_BLOCK_GLSL
#define SIGMA_GRAPHICS_SHADOW_BLOCK_GLSL

layout (std140, binding = 1) uniform shadow_block {
    mat4 light_projection_view_matrix[3];
    vec4 light_frustum_far_plane[3];
};

#endif // SIGMA_GRAPHICS_SHADOW_BLOCK_GLSL
