cmake_minimum_required(VERSION 3.5)
project(sigma-engine)

include(sigma_setup.cmake)

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

target_include_directories(sigma-core PUBLIC include)
target_compile_definitions(sigma-core PUBLIC -DGLM_ENABLE_EXPERIMENTAL -DGLM_FORCE_CTOR_INIT PRIVATE -DSIGMA_EXPORT)
target_link_libraries(sigma-core PUBLIC ${CMAKE_DL_LIBS} Boost::system Boost::filesystem Boost::serialization SDL2::SDL2 Threads::Threads glm jsoncpp_lib_static)

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
add_resources(sigma-core "${CMAKE_CURRENT_SOURCE_DIR}/resources")

target_include_directories(sigma-opengl-graphics PUBLIC include)
target_compile_definitions(sigma-opengl-graphics PRIVATE -DSIGMA_EXPORT)
target_link_libraries(sigma-opengl-graphics PUBLIC sigma-core stb::stb debug-draw glad)

add_library(sigma-tools INTERFACE)
target_include_directories(sigma-tools INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(sigma-tools INTERFACE sigma-core Assimp::assimp)

add_subdirectory(test)
add_subdirectory(benchmark)
