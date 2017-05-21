cmake_minimum_required(VERSION 3.5)
project(sigma-engine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CONAN_LIBCXX STREQUAL "libstdc++11")
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
elseif(CONAN_LIBCXX STREQUAL "libstdc++")
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach()


include(${CMAKE_SOURCE_DIR}/sigma_setup.cmake)
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()


if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
endif()


set(CORE_SOURCES
    src/sigma/window.cpp
    src/sigma/game.cpp
    src/sigma/trackball_controller.cpp
    src/sigma/frustum.cpp
    src/sigma/handle.cpp

    src/sigma/graphics/renderer.cpp
    src/sigma/resource/identifier.cpp
    src/sigma/graphics/material.cpp
    src/sigma/graphics/shader_technique.cpp
    src/sigma/graphics/post_process_effect.cpp
    src/sigma/graphics/static_mesh.cpp

    src/sigma/graphics/opengl/cubemap.cpp
    src/sigma/graphics/opengl/frame_buffer.cpp
    src/sigma/graphics/opengl/geometry_buffer.cpp
    src/sigma/graphics/opengl/shadow_buffer.cpp
    src/sigma/graphics/opengl/material.cpp
    src/sigma/graphics/opengl/post_process_effect.cpp
    src/sigma/graphics/opengl/renderer.cpp
    src/sigma/graphics/opengl/shader.cpp
    src/sigma/graphics/opengl/static_mesh.cpp
    src/sigma/graphics/opengl/texture.cpp
    src/sigma/graphics/opengl/render_buffer.cpp

    src/sigma/util/filesystem.cpp
)
set(CORE_HEADERS
    include/sigma/window.hpp
    include/sigma/world.hpp
    include/sigma/config.hpp
    include/sigma/game.hpp
    include/sigma/trackball_controller.hpp
    include/sigma/transform.hpp
    include/sigma/frustum.hpp
    include/sigma/handle.hpp

    include/sigma/graphics/cubemap.hpp
    include/sigma/graphics/directional_light.hpp
    include/sigma/graphics/material.hpp
    include/sigma/graphics/shader_technique.hpp
    include/sigma/graphics/point_light.hpp
    include/sigma/graphics/post_process_effect.hpp
    include/sigma/graphics/renderer.hpp
    include/sigma/graphics/shader.hpp
    include/sigma/graphics/static_mesh.hpp
    include/sigma/graphics/spot_light.hpp
    include/sigma/graphics/static_mesh_instance.hpp
    include/sigma/graphics/texture.hpp

    include/sigma/graphics/opengl/cubemap.hpp
    include/sigma/graphics/opengl/frame_buffer.hpp
    include/sigma/graphics/opengl/geometry_buffer.hpp
    include/sigma/graphics/opengl/shadow_buffer.hpp
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
    include/sigma/graphics/opengl/render_buffer.hpp

    include/sigma/resource/identifier.hpp
    include/sigma/resource/manager.hpp

    include/sigma/util/compile_time_hash.hpp
    include/sigma/util/filesystem.hpp
    include/sigma/util/glm_serialize.hpp
    include/sigma/util/json_conversion.hpp
    include/sigma/util/variadic.hpp
)

generate_meta_data(CORE_GENERATED_SOURCES ${CORE_HEADERS})
add_library(sigma-core STATIC ${CORE_SOURCES} ${CORE_GENERATED_SOURCES})
target_include_directories(sigma-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/resources)
target_compile_definitions(sigma-core PUBLIC PRIVATE -DSIGMA_EXPORT)
target_link_libraries(sigma-core PUBLIC ${CMAKE_DL_LIBS} ${CONAN_LIBS})

file(GLOB_RECURSE RESOURCES resources resources/*.*)
add_resources(sigma-core PACKAGE_ROOT resources ${RESOURCES})

add_subdirectory(tools/scompiler)


add_subdirectory(simple_game)


enable_testing()
add_subdirectory(test)
add_subdirectory(benchmark)
