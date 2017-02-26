cmake_minimum_required(VERSION 3.2)
project(sopengl)

#find_package(OpenGL REQUIRED)

set(SOURCES
    src/sigma/graphics/opengl/cubemap.cpp
    src/sigma/graphics/opengl/frame_buffer.cpp
    src/sigma/graphics/opengl/geometry_buffer.cpp
    src/sigma/graphics/opengl/material.cpp
    src/sigma/graphics/opengl/post_process_effect.cpp
    src/sigma/graphics/opengl/renderer.cpp
    src/sigma/graphics/opengl/shader.cpp
    src/sigma/graphics/opengl/static_mesh.cpp
    src/sigma/graphics/opengl/texture.cpp
)

set(HEADERS
    include/sigma/graphics/opengl/config.hpp
    include/sigma/graphics/opengl/cubemap.hpp
    include/sigma/graphics/opengl/frame_buffer.hpp
    include/sigma/graphics/opengl/geometry_buffer.hpp
    include/sigma/graphics/opengl/material.hpp
    include/sigma/graphics/opengl/post_process_effect.hpp
    include/sigma/graphics/opengl/render_uniforms.hpp
    include/sigma/graphics/opengl/renderer.hpp
    include/sigma/graphics/opengl/shader_technique.hpp
    include/sigma/graphics/opengl/shader.hpp
    include/sigma/graphics/opengl/static_mesh.hpp
    include/sigma/graphics/opengl/texture.hpp
    include/sigma/graphics/opengl/uniform_buffer.hpp
    include/sigma/graphics/opengl/util.hpp
)

add_library(sopengl SHARED ${SOURCES} ${HEADERS})

target_include_directories(sopengl PUBLIC include resources)#PRIVATE ${OPENGL_INCLUDE_DIR}

target_compile_definitions(sopengl PUBLIC -DSIGMA_SOPENGL_SHARED)
target_compile_options(sopengl PRIVATE -DSIGMA_SOPENGL_EXPORT)

target_link_libraries(sopengl sigma-core ${OPENGL_LIBRARIES})

file(GLOB_RECURSE RESOURCES resources resources/*.*)
add_resources(sopengl PACKAGE_ROOT resources ${RESOURCES})
