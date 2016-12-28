#ifndef SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL
#define SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL

#if defined(SIGMA_ENGINE_POST_PROCESS_EFFECT)
uniform sampler2D in_position;
uniform sampler2D in_diffuse;
uniform sampler2D in_normal;
uniform sampler2D in_image;
out vec4 out_image;
#else
out vec4 position_output;
out vec4 diffuse_output;
out vec4 normal_output;
#endif

#endif // SIGMA_ENGINE_OPENGL_GEOMETRY_BUFFER_GLSL
