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
#find_package(benchmark REQUIRED)
find_package(Boost 1.65.0 COMPONENTS filesystem program_options serialization log REQUIRED)
find_package(glm 0.9.8 REQUIRED)
#find_package(GTest 1.8.0 REQUIRED)
#find_package(GTest REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)
find_package(TIFF REQUIRED)

set(SHADERC_THIRD_PARTY_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty" CACHE STRING "")
add_subdirectory(thirdparty/shaderc EXCLUDE_FROM_ALL)
add_subdirectory(thirdparty/debug-draw EXCLUDE_FROM_ALL)
add_subdirectory(thirdparty/glad EXCLUDE_FROM_ALL)
add_subdirectory(thirdparty/jsoncpp EXCLUDE_FROM_ALL)

add_library(sigma-core STATIC
    src/sigma/frustum.cpp
    src/sigma/game.cpp
    src/sigma/buddy_array_allocator.cpp
    src/sigma/graphics/renderer.cpp
    src/sigma/trackball_controller.cpp
    src/sigma/util/filesystem.cpp
    src/sigma/window.cpp
    src/sigma/graphics/texture.cpp
)

target_include_directories(sigma-core
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(sigma-core
PUBLIC
    -DGLM_ENABLE_EXPERIMENTAL
    -DGLM_FORCE_CTOR_INIT
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
    stdc++fs
)

add_library(sigma-opengl-graphics STATIC
    src/sigma/graphics/opengl/debug_draw_renderer.cpp
    src/sigma/graphics/opengl/cubemap_manager.cpp
    src/sigma/graphics/opengl/geometry_buffer.cpp
    src/sigma/graphics/opengl/renderer.cpp
    src/sigma/graphics/opengl/shader_manager.cpp
    src/sigma/graphics/opengl/static_mesh_manager.cpp
    src/sigma/graphics/opengl/technique_manager.cpp
    src/sigma/graphics/opengl/texture_manager.cpp
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
    debug-draw
    glad
)

add_library(sigma-tools INTERFACE)
target_include_directories(sigma-tools INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(sigma-tools
    INTERFACE
    sigma-core

    PNG::PNG
    TIFF::TIFF
    jpeg
    assimp

    shaderc
)

#add_subdirectory(test)
#add_subdirectory(benchmark)
add_subdirectory(simple_game)
