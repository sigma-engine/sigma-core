cmake_minimum_required(VERSION 3.2)
project(sopengl)

find_package(OpenGL REQUIRED)

set(SOURCES
    src/sigma/opengl/buffer.cpp
    src/sigma/opengl/renderer.cpp
    src/sigma/opengl/frame_buffer.cpp
    src/sigma/opengl/gl_core_4_0.c
    src/sigma/opengl/index_buffer.cpp
    src/sigma/opengl/program.cpp
    src/sigma/opengl/material.cpp
    src/sigma/opengl/shader.cpp
    src/sigma/opengl/texture.cpp
    src/sigma/opengl/static_mesh.cpp
    src/sigma/opengl/texture_2d.cpp
    src/sigma/opengl/vertex_array.cpp
    src/sigma/opengl/vertex_buffer.cpp
    src/sigma/opengl/g_buffer.cpp
)

set(HEADERS
    include/sigma/opengl/buffer.hpp
    include/sigma/opengl/config.hpp
    include/sigma/opengl/renderer.hpp
    include/sigma/opengl/frame_buffer.hpp
    include/sigma/opengl/gl_core_4_0.h
    include/sigma/opengl/index_buffer.hpp
    include/sigma/opengl/program.hpp
    include/sigma/opengl/shader.hpp
    include/sigma/opengl/material.hpp
    include/sigma/opengl/static_mesh.hpp
    include/sigma/opengl/texture_2d.hpp
    include/sigma/opengl/texture.hpp
    include/sigma/opengl/type_info.hpp
    include/sigma/opengl/util.hpp
    include/sigma/opengl/vertex_array.hpp
    include/sigma/opengl/vertex_buffer.hpp
    include/sigma/opengl/g_buffer.hpp
)

add_library(sopengl SHARED ${SOURCES} ${HEADERS})

target_include_directories(sopengl PUBLIC include PRIVATE ${OPENGL_INCLUDE_DIR})

target_compile_definitions(sopengl PRIVATE -DSIGMA_ENGINE_OPENGL_EXPORT)

target_link_libraries(sopengl sigma-core ${OPENGL_LIBRARIES})

add_resource_package(sopengl-resources resources)
add_dependencies(sopengl sopengl-resources)
