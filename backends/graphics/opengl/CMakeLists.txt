cmake_minimum_required(VERSION 3.2)
project(opengl)

find_package(OpenGL REQUIRED)

set(SOURCES
    src/sigmafive/opengl/buffer.cpp
    src/sigmafive/opengl/context.cpp
    src/sigmafive/opengl/frame_buffer.cpp
    src/sigmafive/opengl/gl_core_4_0.c
    src/sigmafive/opengl/index_buffer.cpp
    src/sigmafive/opengl/program.cpp
    src/sigmafive/opengl/material.cpp
    src/sigmafive/opengl/shader.cpp
    src/sigmafive/opengl/texture.cpp
    src/sigmafive/opengl/static_mesh.cpp
    src/sigmafive/opengl/texture_2d.cpp
    src/sigmafive/opengl/vertex_array.cpp
    src/sigmafive/opengl/vertex_buffer.cpp
)

set(HEADERS
    include/sigmafive/opengl/buffer.hpp
    include/sigmafive/opengl/config.hpp
    include/sigmafive/opengl/context.hpp
    include/sigmafive/opengl/frame_buffer.hpp
    include/sigmafive/opengl/gl_core_4_0.h
    include/sigmafive/opengl/index_buffer.hpp
    include/sigmafive/opengl/program.hpp
    include/sigmafive/opengl/shader.hpp
    include/sigmafive/opengl/material.hpp
    include/sigmafive/opengl/static_mesh.hpp
    include/sigmafive/opengl/texture_2d.hpp
    include/sigmafive/opengl/texture.hpp
    include/sigmafive/opengl/type_info.hpp
    include/sigmafive/opengl/util.hpp
    include/sigmafive/opengl/vertex_array.hpp
    include/sigmafive/opengl/vertex_buffer.hpp
)

add_library(opengl STATIC ${SOURCES} ${HEADERS})

target_include_directories(opengl PUBLIC include PRIVATE ${OPENGL_INCLUDE_DIR})

target_compile_definitions(opengl PRIVATE -DSIGMA_FIVE_OPENGL_EXPORT)

target_link_libraries(opengl engine ${OPENGL_LIBRARIES} pthread ${CONAN_LIBS})

add_resource_package(opengl-resources resources)
add_dependencies(opengl opengl-resources)
