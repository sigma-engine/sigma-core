cmake_minimum_required(VERSION 2.8.12)
project(opengl_context)

add_package(cppbr-opengl)

set(SOURCES
    src/context.cpp
    src/g_buffer.cpp
    src/material.cpp
    src/static_mesh.cpp
    src/static_mesh_manager.cpp
)

set(HEADERS
    include/context.hpp
    include/g_buffer.hpp
    include/material.hpp
    include/static_mesh.hpp
    include/static_mesh_manager.hpp
)

add_library(opengl_context SHARED ${SOURCES} ${HEADERS})

target_include_directories(opengl_context PUBLIC include)

target_compile_definitions(opengl_context
PUBLIC
    -D$<$<CONFIG:Debug>:opengl_context_DEBUG>
PRIVATE
    -Dopengl_context_EXPORT
)

if(NOT MSVC)
    target_compile_options(opengl_context PUBLIC -std=c++14 -pedantic -fPIC)
endif()

target_link_libraries(opengl_context cppbr-opengl sigma-five-engine)

set_target_properties(opengl_context
			PROPERTIES
			PREFIX ""
			LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
			RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
