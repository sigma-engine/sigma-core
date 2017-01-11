cmake_minimum_required(VERSION 3.2)
project(sopengl)

find_package(OpenGL REQUIRED)

set(SOURCES
    src/sigma/opengl/cubemap.cpp
    src/sigma/opengl/frame_buffer.cpp
    src/sigma/opengl/geometry_buffer.cpp
    src/sigma/opengl/gl_core_4_2.c
    src/sigma/opengl/material.cpp
    src/sigma/opengl/post_process_effect.cpp
    src/sigma/opengl/renderer.cpp
    src/sigma/opengl/scene.cpp
    src/sigma/opengl/shader.cpp
    src/sigma/opengl/static_mesh.cpp
    src/sigma/opengl/texture.cpp
)

set(HEADERS
    include/sigma/opengl/config.hpp
    include/sigma/opengl/cubemap.hpp
    include/sigma/opengl/frame_buffer.hpp
    include/sigma/opengl/geometry_buffer.hpp
    include/sigma/opengl/gl_core_4_2.h
    include/sigma/opengl/material.hpp
    include/sigma/opengl/post_process_effect.hpp
    include/sigma/opengl/render_uniforms.hpp
    include/sigma/opengl/renderer.hpp
    include/sigma/opengl/scene.hpp
    include/sigma/opengl/shader_technique.hpp
    include/sigma/opengl/shader.hpp
    include/sigma/opengl/static_mesh.hpp
    include/sigma/opengl/texture.hpp
    include/sigma/opengl/uniform_buffer.hpp
    include/sigma/opengl/util.hpp
)

add_library(sopengl SHARED ${SOURCES} ${HEADERS})

target_include_directories(sopengl PUBLIC include resources PRIVATE ${OPENGL_INCLUDE_DIR})

target_compile_definitions(sopengl PUBLIC -DSIGMA_SOPENGL_SHARED)
target_compile_options(sopengl PRIVATE -DSIGMA_SOPENGL_EXPORT)

target_link_libraries(sopengl sigma-core ${OPENGL_LIBRARIES})

file(GLOB_RECURSE SHADER_SOURCES resources ${SIGMA_ENGINE_SHADER_GLOB})
file(GLOB_RECURSE TEXTURE_SOURCES resources ${SIGMA_ENGINE_TEXTURE_GLOB})
file(GLOB_RECURSE MATERIAL_SOURCES resources ${SIGMA_ENGINE_MATERIAL_GLOB})
file(GLOB_RECURSE MODEL_SOURCES resources ${SIGMA_ENGINE_MODEL_GLOB})
add_resources(sopengl PACKAGE_ROOT resources ${SHADER_SOURCES} ${TEXTURE_SOURCES} ${MATERIAL_SOURCES} ${MODEL_SOURCES})
