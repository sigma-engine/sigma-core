cmake_minimum_required(VERSION 3.5)
project(sigma-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/lib")
endforeach()

SET(Boost_USE_STATIC_LIBS ON)
#find_package(GMock REQUIRED)
find_package(Assimp 4.0.0 REQUIRED)
find_package(benchmark REQUIRED)
find_package(Boost 1.65.0 COMPONENTS filesystem program_options serialization wave log REQUIRED)
find_package(glm 0.9.8 REQUIRED)
find_package(GTest 1.8.0 REQUIRED)
find_package(GTest REQUIRED)
find_package(JPEG REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(PNG REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)
find_package(TIFF REQUIRED)


add_library(sigma-core STATIC
    src/sigma/frustum.cpp
    src/sigma/game.cpp
    src/sigma/graphics/renderer.cpp
    src/sigma/handle.cpp
    src/sigma/trackball_controller.cpp
    src/sigma/util/filesystem.cpp
    src/sigma/window.cpp
)

target_include_directories(sigma-core
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(sigma-core
    PRIVATE
    -DSIGMA_EXPORT
)

target_link_libraries(sigma-core
    PUBLIC
    ${CMAKE_DL_LIBS}
    Boost::boost
    Boost::filesystem
    Boost::serialization
    Threads::Threads
    glm
    jsoncpp
    SDL2
)

add_library(sigma-opengl-graphics STATIC
    src/sigma/graphics/opengl/debug_draw_renderer.cpp
    src/sigma/graphics/opengl/cubemap.cpp
    src/sigma/graphics/opengl/frame_buffer.cpp
    src/sigma/graphics/opengl/geometry_buffer.cpp
    src/sigma/graphics/opengl/material.cpp
    src/sigma/graphics/opengl/post_process_effect.cpp
    src/sigma/graphics/opengl/render_buffer.cpp
    src/sigma/graphics/opengl/renderer.cpp
    src/sigma/graphics/opengl/shader.cpp
    src/sigma/graphics/opengl/shadow_buffer.cpp
    src/sigma/graphics/opengl/static_mesh.cpp
    src/sigma/graphics/opengl/technique.cpp
    src/sigma/graphics/opengl/texture.cpp
)

target_include_directories(sigma-opengl-graphics
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(sigma-opengl-graphics
    PRIVATE
    -DSIGMA_EXPORT
)

target_link_libraries(sigma-opengl-graphics
    PUBLIC
    sigma-core
    glad
    debug-draw
)

add_subdirectory(test)
add_subdirectory(benchmark)
add_subdirectory(tools/pakcc)
add_subdirectory(simple_game)
