cmake_minimum_required(VERSION 2.8.12)
project(sigmafive)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(jsoncpp REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

set(SOURCES
    src/sigmafive/application.cpp
    src/sigmafive/context.cpp
    src/sigmafive/object.cpp

    src/sigmafive/system/window.cpp
    src/sigmafive/system/resource.cpp
    src/sigmafive/system/resource_manager.cpp

    src/sigmafive/graphics/graphics_system.cpp
    src/sigmafive/graphics/static_mesh.cpp

    src/sigmafive/graphics/opengl/gl_core_4_0.cpp
    src/sigmafive/graphics/opengl/buffer.cpp
    src/sigmafive/graphics/opengl/frame_buffer.cpp
    src/sigmafive/graphics/opengl/index_buffer.cpp
    src/sigmafive/graphics/opengl/material.cpp
    src/sigmafive/graphics/opengl/ogl_graphics_system.cpp
    src/sigmafive/graphics/opengl/program.cpp
    src/sigmafive/graphics/opengl/scene_renderer.cpp
    src/sigmafive/graphics/opengl/shader.cpp
    src/sigmafive/graphics/opengl/static_mesh.cpp
    src/sigmafive/graphics/opengl/static_mesh_manager.cpp
    src/sigmafive/graphics/opengl/texture_2d.cpp
    src/sigmafive/graphics/opengl/vertex_array.cpp
    src/sigmafive/graphics/opengl/vertex_buffer.cpp

    src/sigmafive/game/scene.cpp
    src/sigmafive/game/entity.cpp
    src/sigmafive/game/component.cpp
    src/sigmafive/game/entity_world.cpp
    src/sigmafive/game/entity_manager.cpp
    src/sigmafive/game/component_pool.cpp
    src/sigmafive/game/component_bitset_manager.cpp
    src/sigmafive/game/component_system.cpp
    src/sigmafive/game/component_manager.cpp
    src/sigmafive/game/transform_component.cpp
    src/sigmafive/game/static_mesh_component.cpp
    src/sigmafive/game/component_system_manager.cpp
    src/sigmafive/game/static_mesh_component_system.cpp
)
set(HEADERS
    include/sigmafive/application.hpp
    include/sigmafive/context.hpp
    include/sigmafive/object.hpp

    include/sigmafive/type_info.hpp
    include/sigmafive/compile_time_hash.hpp
    include/sigmafive/serialization/unique_ptr.hpp

    include/sigmafive/math/mat2.hpp
    include/sigmafive/math/mat3.hpp
    include/sigmafive/math/mat4.hpp
    include/sigmafive/math/vec2.hpp
    include/sigmafive/math/vec3.hpp
    include/sigmafive/math/vec4.hpp
    include/sigmafive/math/utils.hpp
    include/sigmafive/math/quaternion.hpp

    include/sigmafive/system/window.hpp
    include/sigmafive/system/resource.hpp
    include/sigmafive/system/resource_manager.hpp

    include/sigmafive/graphics/graphics_system.hpp
    include/sigmafive/graphics/static_mesh.hpp

    include/sigmafive/graphics/opengl/gl_core_4_0.hpp
    include/sigmafive/graphics/opengl/buffer.hpp
    include/sigmafive/graphics/opengl/frame_buffer.hpp
    include/sigmafive/graphics/opengl/index_buffer.hpp
    include/sigmafive/graphics/opengl/material.hpp
    include/sigmafive/graphics/opengl/ogl_graphics_system.hpp
    include/sigmafive/graphics/opengl/program.hpp
    include/sigmafive/graphics/opengl/scene_renderer.hpp
    include/sigmafive/graphics/opengl/shader.hpp
    include/sigmafive/graphics/opengl/static_mesh.hpp
    include/sigmafive/graphics/opengl/static_mesh_manager.hpp
    include/sigmafive/graphics/opengl/texture_2d.hpp
    include/sigmafive/graphics/opengl/util.hpp
    include/sigmafive/graphics/opengl/vertex_array.hpp
    include/sigmafive/graphics/opengl/vertex_buffer.hpp

    include/sigmafive/game/scene.hpp
    include/sigmafive/game/entity.hpp
    include/sigmafive/game/component.hpp
    include/sigmafive/game/entity_world.hpp
    include/sigmafive/game/entity_manager.hpp
    include/sigmafive/game/component_pool.hpp
    include/sigmafive/game/component_bitset_manager.hpp
    include/sigmafive/game/component_system.hpp
    include/sigmafive/game/component_manager.hpp
    include/sigmafive/game/transform_component.hpp
    include/sigmafive/game/static_mesh_component.hpp
    include/sigmafive/game/component_system_manager.hpp
    include/sigmafive/game/static_mesh_component_system.hpp
)

add_library(sigmafive STATIC ${SOURCES} ${HEADERS})
target_include_directories(sigmafive
PUBLIC
    include
    ${Boost_INCLUDE_DIRS}
    ${jsoncpp_INCLUDE_DIRS}
PRIVATE
    ${OPENGL_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)
if(NOT MSVC)
    target_compile_options(sigmafive
    PUBLIC
        -std=c++11
    )
endif()

target_link_libraries(sigmafive ${Boost_LIBRARIES}  ${jsoncpp_LIBRARIES} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})

#add_subdirectory(tests)
